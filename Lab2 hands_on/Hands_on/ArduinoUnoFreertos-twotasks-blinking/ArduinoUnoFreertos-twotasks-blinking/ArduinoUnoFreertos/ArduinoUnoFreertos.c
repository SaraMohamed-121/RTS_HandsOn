#include <avr/io.h>
#include <util/delay.h>
#include "FreeRTOS.h"
#include "task.h"

#define LIGHT_PIN (1 << PB0)
#define FAN_PIN (1 << PB1)

void vLightTask(void *pvParameters);

void vFanTask(void *pvParameters);

int main(void) {
	//1- Initialize hardware, ports, and FreeRTOS

	//2- create tasks with correct priorities assignation
	
	vTaskStartScheduler();

	for(;;);
}
