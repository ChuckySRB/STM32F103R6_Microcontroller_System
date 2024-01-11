/*
 * caslav.c
 *
 *  Created on: Jan 9, 2024
 *      Author: ChuckySRB
 */


#include "caslav.h"

void writeLCD(unsigned addr, const char *str) {
	LCD_Use(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | addr);
	for (size_t i = 0; i < strlen(str); ++i) {
		LCD_Use(LCD_DATA, str[i]);
	}
}

void SpamTask(void* params){
	while (1){
		vTaskDelay(pdMS_TO_TICKS(200));
		Transmit_Char('C');
		Transmit_String("uckySRB :)");
		Transmit_Char('\r');
		writeLCD(0x00, "chucky");
		writeLCD(0x40, "SRB");
		writeLCD(0x10, "SRB");
	}
}

void caki(){
	uart_init();
	lcd_init();
	xTaskCreate(SpamTask, "Spam", 100, NULL, 2, NULL);
}
