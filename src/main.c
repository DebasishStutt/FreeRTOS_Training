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
#define NOTIFY_BASED_ACTIVATION
//#define NOTIFY_BY_POLLING
#define NOTIFY_FROM_ISR

#define FALSE 0
#define TRUE 1


TaskHandle_t task1_Handle;
TaskHandle_t task2_Handle;
TaskHandle_t taskLED_Handle;
TaskHandle_t taskBUTTON_Handle;

#ifdef ALLOW_TASK1_AND_TASK2
void task1Handler(void* params);
void task2Handler(void* params);
#endif

#ifdef NOTIFY_BY_POLLING
void taskButtonNotifyType(void* params);
#endif

#ifndef NOTIFY_BASED_ACTIVATION
uint8_t button_status_flag = FALSE;
void taskButton(void* params);
#endif

void taskLed(void* params);
void rtosDelay(uint32_t delay_in_ms);




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

#ifdef ALLOW_TASK1_AND_TASK2
	xTaskCreate(task1_Handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIO, &task1_Handle);
	xTaskCreate(task2_Handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, TASK2_PRIO, &task2_Handle);
#endif

	xTaskCreate(taskLed, "LED-TASK", 500, NULL, TASK1_PRIO, &taskLED_Handle);

#ifdef TOGGLE_BY_POLLING // default: button signal via interrupt
	xTaskCreate(task_Button, "BUTTON-TASK", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIO, &taskBUTTON_Handle);
#elif defined(NOTIFY_BY_POLLING)
	xTaskCreate(taskButtonNotifyType, "BUTTON-TASK", 500, NULL, TASK1_PRIO, &taskBUTTON_Handle);
#endif

	vTaskStartScheduler();

	for(;;);
}

#ifdef NOTIFY_BY_POLLING
void taskButtonNotifyType(void* params) {
	while(1) {

		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == 0) {
			rtosDelay(500);
			xTaskNotify(taskLED_Handle, 0x00, eIncrement);
		}
	}

}
#endif

#ifdef NOTIFY_FROM_ISR
void buttonInterruptHandler() {
#ifndef NOTIFY_FROM_ISR
	button_status_flag ^= 0x01;
#endif
	BaseType_t prioWokenTask;
	xTaskNotifyFromISR(taskLED_Handle, 0x00, eIncrement, &prioWokenTask);
}
#endif

#ifdef TOGGLE_BY_POLLING
void taskButton(void* params) {
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
#endif

void taskLed(void* params) {
	while(1) {
#ifndef NOTIFY_BASED_ACTIVATION
		if(button_status_flag == TRUE) {
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
			printMsg("LED Task: LED ON \r\n");
		} else {
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
			printMsg("LED Task: LED OFF \r\n");
		}
		vTaskDelay(100);
#else
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5)) {
		printMsg("Led is currently ON... \r\n");
	} else {
		printMsg("Led is currently OFF... \r\n");
	}
	if (xTaskNotifyWait(0x01, 0x01, NULL, portMAX_DELAY) == pdTRUE) {
		printMsg("TaskNotify received...\r\n");
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	}
#endif
	}

}

#ifdef ALLOW_TASK1_AND_TASK2
void task1Handler(void* params) {
	while(1) {
		//printf("From task1\n");
		printMsg("Message from Task1 \n\r");
		vTaskDelay(100);
	}
}

void task2Handler(void* params) {
	while(1) {
		//printf("From task2\n");
		printMsg("Message from Task2 \n\r");
		vTaskDelay(10);
	}
}
#endif
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	gpio_button_pins.GPIO_Mode = GPIO_Mode_IN;
	gpio_button_pins.GPIO_OType = GPIO_OType_PP;
	gpio_button_pins.GPIO_Pin = GPIO_Pin_13;
	gpio_button_pins.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_button_pins.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(GPIOC, &gpio_button_pins);
#ifdef NOTIFY_FROM_ISR
	// interrupt setup
	//1. Config system to connect EXT13 to PC13 --> SYSCFG
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	//2. Setup/config the EXTI block
	EXTI_InitTypeDef extiInit;

	extiInit.EXTI_Line = EXTI_Line13;
	extiInit.EXTI_LineCmd = ENABLE;
	extiInit.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger = EXTI_Trigger_Falling;

	EXTI_Init(&extiInit);

	//3. NVIC setup
	NVIC_SetPriority(EXTI15_10_IRQn, 5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif
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

#ifdef NOTIFY_FROM_ISR
void EXTI15_10_IRQHandler(void) {
	//1. CLear interrupt
	EXTI_ClearITPendingBit(EXTI_Line13);

	//2.
	//rtosDelay(500);
	buttonInterruptHandler();
}
#endif

void rtosDelay(uint32_t delay_in_ms) {
	uint32_t currentTickCount = xTaskGetTickCount();

	while(xTaskGetTickCount() < (currentTickCount + pdMS_TO_TICKS(delay_in_ms)));
}
