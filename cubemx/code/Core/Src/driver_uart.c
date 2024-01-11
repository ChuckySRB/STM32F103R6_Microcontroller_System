/*
 * driver_uart.c
 *
 *  Created on: Jan 11, 2024
 *      Author: ChuckySRB
 */
#include "driver_uart.h"


// staticni red
static QueueHandle_t TransmitQueue;
// staticna rucka taska
static TaskHandle_t TransmitTask;
static TaskHandle_t ReceiveTask;
// mutex
static SemaphoreHandle_t TransmitMutex;



// <===========================================================================>
// TRANSMIT

static void uart_transmit(void *parameters){
	uint8_t buffer;
	while(1){
		// Izvaditi iz reda stvari koje cekaju na slanje
		xQueueReceive(TransmitQueue, &buffer, portMAX_DELAY);
		// poslati ga preko uar comande
		HAL_UART_Transmit_IT(&huart1, &buffer, sizeof(uint8_t));
		// blokirati se sa semaforom dok se u prekidnoj rutina ne javi da je slanje završeno
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}

}


// Prekidna rutina koja se aktivira kada je Transmition complited
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == huart1.Instance){
		// obavestiti task da je transmition complete
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(TransmitTask, &woken);
		portYIELD_FROM_ISR(woken);
	}
}


// <===========================================================================>
// RECEIVE

static void uart_receive(void *parameters){
	uint8_t buffer;
	while(1){
		// poslati ga preko uar comande
		HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));
		// blokirati se sa semaforom dok se u prekidnoj rutina ne javi da je slanje završeno
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}

}


// Prekidna rutina koja se aktivira kada je Receive complited
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == huart1.Instance){
		// obavestiti task da je receive complete
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(ReceiveTask, &woken);
		portYIELD_FROM_ISR(woken);
	}
}



//<============================================================================>
// INIT

void uart_init(){
	// napraviti task
	xTaskCreate(uart_transmit, "Transmit a task", 128, NULL, 3, &TransmitTask);
	xTaskCreate(uart_receive, "Receive a task", 128, NULL, 3, &ReceiveTask);
	// napraviti red
	TransmitQueue = xQueueCreate(128, sizeof(uint8_t));
	// napraviti semafora
	TransmitMutex = xSemaphoreCreateMutex();
}

// < ==========================================================================>
// TRANSMIT UTILS

void UART_Transmit(uint8_t byte) {
	xQueueSendToBack(TransmitQueue, &byte, portMAX_DELAY);
}

void Transmit_Char(char c){
	xSemaphoreTake(TransmitMutex, portMAX_DELAY);
	UART_Transmit(c);
	xSemaphoreGive(TransmitMutex);
}

void Transmit_String(const char *s){
	int lenght = strlen(s);
	xSemaphoreTake(TransmitMutex, portMAX_DELAY);
	for (int i = 0; i < lenght; i++){
			UART_Transmit(s[i]);
	}
	xSemaphoreGive(TransmitMutex);
}



