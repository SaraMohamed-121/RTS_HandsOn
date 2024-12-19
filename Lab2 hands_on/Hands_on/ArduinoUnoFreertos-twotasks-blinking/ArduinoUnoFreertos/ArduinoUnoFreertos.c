#include <avr/io.h>
#include <util/delay.h>
#include "FreeRTOS.h"
#include "task.h"

#define LIGHT_PIN PB0
#define FAN_PIN PB1

void vLightTask(void *pvParameters);
void vFanTask(void *pvParameters);

int main(void) {
	//1- Initialize hardware, ports, and FreeRTOS
	DDRB=0xFF;

	//2- create tasks with correct priorities assignation
	xTaskCreate(vLightTask,"vLightTask",256,NULL,1,NULL);
	xTaskCreate(vFanTask,"vFanTask",256,NULL,2,NULL);	
	vTaskStartScheduler();

	for(;;);
}

void vLightTask(void *pvParameters){
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1){
		PORTB |= (1 << LIGHT_PIN);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));

		PORTB &= ~(1 << LIGHT_PIN);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
		//vTaskDelayUntil(&xLastWakeTime,500/portTICK_RATE_MS);
}
}
void vFanTask(void *pvParameters){
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while (1)
	{
		PORTB |= (1 << FAN_PIN);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));

		PORTB &= ~(1 << FAN_PIN);
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
	}
	
}