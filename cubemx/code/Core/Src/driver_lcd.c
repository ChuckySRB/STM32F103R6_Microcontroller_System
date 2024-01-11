/*
 * driver_lcd.c
 *
 *  Created on: Jan 11, 2024
 *      Author: ChuckySRB
 */

#include "driver_lcd.h"

#include "gpio.h"

#include "FreeRTOS.h"

#define LCD_ENABLE_BIT 0x40

QueueHandle_t LCDQueue;
LCD_Command cmd;

static void LCD_Write(LCD_CommandReg reg, LCD_CommandVal val){
	GPIOC->ODR = ((reg) << 4) | (val & 0x0F);
	GPIOC->ODR |= LCD_ENABLE_BIT;
	GPIOC->ODR &= ~LCD_ENABLE_BIT;
}

static void LCD_WriteCommand(LCD_CommandReg reg, LCD_CommandVal val){
	LCD_Write(reg, val >> 4);
	LCD_Write(reg, val >> 0);
	vTaskDelay(pdMS_TO_TICKS(2));
}

static void LCD_Task(void *parameter){
	vTaskDelay(pdMS_TO_TICKS(20));
	LCD_Write(LCD_INSTRUCTION, (LCD_FUNCTION_SET_INSTRUCTION) >> 4);
	vTaskDelay(pdMS_TO_TICKS(2));
	LCD_WriteCommand(LCD_INSTRUCTION ,LCD_FUNCTION_SET_INSTRUCTION |
			LCD_FUNCTION_SET_4_BIT_INTERFACE |
			LCD_FUNCTION_SET_2_LINE |
			LCD_FUNCTION_SET_5x8_DOTS);
	LCD_WriteCommand(LCD_INSTRUCTION , LCD_CONTROL_DISPLAY_ON | LCD_CONTROL_INSTRUCTION);
	LCD_WriteCommand(LCD_INSTRUCTION, LCD_ENTRY_MODE_INSTRUCTION | LCD_ENTRY_MODE_INC_ADR);
	LCD_WriteCommand(LCD_INSTRUCTION, LCD_RETURN_HOME_INSTRUCTION);



	while(1){
		xQueueReceive(LCDQueue, &cmd, portMAX_DELAY);
		LCD_WriteCommand(cmd.reg, cmd.val);
	}
}

void lcd_init(){
	xTaskCreate(LCD_Task, "LCD", 128, NULL, 3, NULL);
	LCDQueue = xQueueCreate(128, sizeof(LCD_Command));
}


void LCD_Use(LCD_CommandReg reg, LCD_CommandVal val){
	cmd.reg = reg;
	cmd.val = val;
	xQueueSendToBack(LCDQueue,&cmd, portMAX_DELAY);
}

