#include "FreeRTOS.h"
#include "task.h"
#include <avr/io.h>
#include <util/delay.h>

// declaration of two methods
void led1(void* pvParameters);
void led2(void* pvParameters);
//-----------------------------------------------------------

int main(void)
{ 
	/*
	Create two tasks using xTaskCreate()

	portBASE_TYPE  xTaskCreate(
	pdTASK_CODE 	pvTaskCode,
	const signed char 	* const pcName,
	unsigned short 		usStackDepth,
	void 			*pvParameters,
	unsigned portBASE_TYPE uxPriority,
	xTaskHandle 		*pxCreatedTask
	);
	*/
	

	DDRB=0xFF;
	xTaskCreate(led1,"led1",256,NULL,1,NULL);
	xTaskCreate(led2,"led2",256,NULL,1,NULL);
	vTaskStartScheduler();
	for (;;)
	;;

	return 0;
}

//-----------------------------------------------------------

// method1 implementation 
void led1(void* pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (1){
		PORTB|=(1<<PORTB3);
		vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(1000));
	    PORTB &= ~(1 << PORTB3);
	    vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(1000));
	}
	vTaskDelete(NULL);
}




// method2 implementation 
void led2(void* pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (1){
		PORTB|=(1<<PORTB5);
		vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(500));
		//vTaskDelayUntil(&xLastWakeTime, (500/portTICK_RATE_MS)); //configTICK_RATE_HZ too high (e.g., 10,000) 
		PORTB&=~(1<<PORTB5);
		vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(500));
	}
	vTaskDelete(NULL);
}