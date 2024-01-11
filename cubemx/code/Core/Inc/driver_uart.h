/*
 * driver_uart.h
 *
 *  Created on: Jan 11, 2024
 *      Author: ChuckySRB
 */

#ifndef CORE_INC_DRIVER_UART_H_
#define CORE_INC_DRIVER_UART_H_

#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <string.h>



void extern uart_init();
void extern Transmit_Char(char c);
void extern Transmit_String(const char *s);

#endif /* CORE_INC_DRIVER_UART_H_ */
