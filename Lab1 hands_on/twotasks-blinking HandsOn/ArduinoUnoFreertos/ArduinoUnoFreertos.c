/*
 * ArduinoUnoFreertos.c
 *
 * Created: 10/01/2017 04:49:23 م
 *  Author: D.Randa and Hisham
 */ 

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <util/delay.h>

// declaration of two methods

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
	
	
	vTaskStartScheduler();

	for (;;)
	;;

	return 0;
}

//-----------------------------------------------------------

// method1 implementation 





// method2 implementation 