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
	PORTD ^= 0xFF; //(1<<PORT5);
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
		in=USART_receive();
		USART_sendstr(in);
		if(in =='s'){
			xTimerStop(Timer_Function1 ,1000);
		}
		else if(in == 'r' )
		{
			xTimerReset(Timer_Function1, 1000);
		}
		else if(in == 'd' )
		{
			period = period * 2 ;
			xTimerChangePeriod(Timer_Function1, period, 1000);
		}
		else if(in == 'i' )
		{
			period = period / 2 ;
			xTimerChangePeriod(Timer_Function1, period, 1000);
		}

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
	Timer_Function1= xTimerCreate("timer1",100, pdTRUE,(void *) 0, MyTimer);
	xTimerStart(Timer_Function1, 0 );
	xTaskCreate(Task1, "task1",256, NULL,1, NULL);
	vTaskStartScheduler();
	
	return 0;
}
