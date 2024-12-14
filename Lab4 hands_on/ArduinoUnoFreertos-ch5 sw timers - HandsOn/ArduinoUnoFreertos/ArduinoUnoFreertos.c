
/* FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "USART.h"
#define F_CPU 16000000UL
TimerHandle_t Timer_Function1;
void MyTimer( TimerHandle_t Timer_Function1 )
{
	/*toggle the led on portD pin 5*/
}
void Task1(void* para)
{
	char in;
	uint16_t period = 100; //initial timer period in ticks
	while(1)
	{
		
	    /*
		1. receive a char from the user,then display it on terminal window. 
		
		2. implement the mentioned conditions.
		
		*/

	}
}
int main(void) {
	USART_init();
	DDRD = 0xFF;
	
	/*
	1. create a auto-reload timer with periodic ticks = 100.
	
	2. start the timer.
	
	3. create a task which controls the frequency of the timer.
	
	4. start the scheduler.
	
	*/
	
	return 0;
}
