/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include<stdio.h>
#include<stdint.h>
#include<string.h>

#include "stm32f4xx.h"
			
#include<FreeRTOS.h>
#include<task.h>


//#define ENABLE_SEMIHOSTING
#define INTERRUPT_BASED_LED
#define TASK1_PRIO 2
#define TASK2_PRIO TASK1_PRIO

#define FALSE 0
#define TRUE 1


TaskHandle_t task1_Handle;
TaskHandle_t task2_Handle;
TaskHandle_t taskLED_Handle;
TaskHandle_t taskBUTTON_Handle;

void task1_Handler(void* params);
void task2_Handler(void* params);

void task_Button(void* params);
void task_LED(void* params);


uint8_t button_status_flag = FALSE;


#ifdef ENABLE_SEMIHOSTING
extern void initialise_monitor_handles();
#endif

void prvSetupHardware();
void prvSetupUART();
void printMsg(char* msg);
void prvSetupButton();
void prvSetupLed();

int main(void)
{
#ifdef ENABLE_SEMIHOSTING
	initialise_monitor_handles();
#endif
	RCC_DeInit();
	SystemCoreClockUpdate();

	prvSetupHardware();

	printMsg("Hello!!!! Yes Works\n\r");

	//xTaskCreate(task1_Handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIO, &task1_Handle);
	//xTaskCreate(task2_Handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, TASK2_PRIO, &task2_Handle);

	xTaskCreate(task_LED, "LED-TASK", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIO, &taskLED_Handle);
	xTaskCreate(task_Button, "BUTTON-TASK", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIO, &taskBUTTON_Handle);

	vTaskStartScheduler();

	for(;;);
}


void task_Button(void* params) {
	while(1) {

		uint8_t buttonValue = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

		if(buttonValue) {
			// button not pressed
			button_status_flag = FALSE;
		} else {
			// button pressed
			printMsg("Button Task: Button Pressed \r\n");
			button_status_flag = TRUE;
		}

		vTaskDelay(10);
	}
}

void task_LED(void* params) {
	while(1) {

		if(button_status_flag == TRUE) {
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
			printMsg("LED Task: LED ON \r\n");
		} else {
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
			//printMsg("LED Task: LED OFF \r\n");
		}
		vTaskDelay(100);
	}
}


void task1_Handler(void* params) {
	while(1) {
		//printf("From task1\n");
		printMsg("Message from Task1 \n\r");
		vTaskDelay(100);
	}
}

void task2_Handler(void* params) {
	while(1) {
		//printf("From task2\n");
		printMsg("Message from Task2 \n\r");
		vTaskDelay(10);
	}
}

void prvSetupHardware() {
	prvSetupUART();
	prvSetupButton();
	prvSetupLed();
}

void printMsg(char* msg) {
	for(uint32_t i = 0; i < strlen(msg); i++) {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}
}


void prvSetupUART() {
	GPIO_InitTypeDef gpio_uart_pins;

	memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins));

	// 1. Enable the USART2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// 2. Config PA2 and PA3 as TX and RX respectively
	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_uart_pins.GPIO_OType= GPIO_OType_PP;
    gpio_uart_pins.GPIO_Speed = GPIO_High_Speed;

	GPIO_Init(GPIOA, &gpio_uart_pins);

	// 3. Confg alternate function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //PA2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //PA3

	// 4. Config USART parameters
	USART_InitTypeDef usart2_init_struct;

	memset(&usart2_init_struct, 0, sizeof(usart2_init_struct));

	usart2_init_struct.USART_BaudRate = 115200;
	usart2_init_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart2_init_struct.USART_Parity = USART_Parity_No;
	usart2_init_struct.USART_StopBits = USART_StopBits_1;
	usart2_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart2_init_struct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &usart2_init_struct);

	USART_Cmd(USART2, ENABLE);
}

void prvSetupButton() {
	GPIO_InitTypeDef gpio_button_pins;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	gpio_button_pins.GPIO_Mode = GPIO_Mode_IN;
	gpio_button_pins.GPIO_OType = GPIO_OType_PP;
	gpio_button_pins.GPIO_Pin = GPIO_Pin_13;
	gpio_button_pins.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_button_pins.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(GPIOC, &gpio_button_pins);

}

void prvSetupLed() {
	GPIO_InitTypeDef gpio_led_pins;

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); not needed as already done in UART setup

	gpio_led_pins.GPIO_Mode = GPIO_Mode_OUT;
	gpio_led_pins.GPIO_OType = GPIO_OType_PP;
	gpio_led_pins.GPIO_Pin = GPIO_Pin_5;
	gpio_led_pins.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_led_pins.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(GPIOA, &gpio_led_pins);

}
