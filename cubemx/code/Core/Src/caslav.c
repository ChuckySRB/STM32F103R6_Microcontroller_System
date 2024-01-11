/*
 * caslav.c
 *
 *  Created on: Jan 9, 2024
 *      Author: ChuckySRB
 */


#include "caslav.h"


void SpamTask(void* params){
	while (1){
		vTaskDelay(pdMS_TO_TICKS(200));
		Transmit_Char('C');
		Transmit_String("uckySRB :)");
		Transmit_Char('\r');
	}
}

void caki(){
	uart_init();
	xTaskCreate(SpamTask, "Spam", 100, NULL, 2, NULL);
}
