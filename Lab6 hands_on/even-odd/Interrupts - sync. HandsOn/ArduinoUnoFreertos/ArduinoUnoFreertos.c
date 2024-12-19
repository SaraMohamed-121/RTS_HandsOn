#define TrackSize 5
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "USART.h"
#include "wiring.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

static void vTimerISR();

/* The tasks to be created.  Two instances are created of the sender task while
only a single instance is created of the receiver task. */
static void vOddTask( void *pvParameters );
static void vEvenTask( void *pvParameters );

#define LONG_TIME 0xffff
#define TICKS_TO_WAIT    10

//-----------------------------------------------------------
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;

SemaphoreHandle_t sem;
int main(void)
{ 
  cli();
  attachInterrupt(0, vTimerISR, 1); 
  USART_init();
  sem =  xSemaphoreCreateBinary();
  if(sem != NULL){
    DDRC = 0xff;
	xTaskCreate( vOddTask, "Odd", 256, NULL, 1, &TaskHandle_1 );
	xTaskCreate( vEvenTask, "Even", 256, NULL, 3, &TaskHandle_2 );
	sei();
	vTaskStartScheduler();
	 }
  else
	   USART_sendstr("Semaphore not Created Correctly !!");
	 
	for( ;; );
	return 0;
	
	
}
unsigned long lastInterrupt;


static void vTimerISR()
{
	if(xTaskGetTickCount() - lastInterrupt > 500) // we set a 10ms no-interrupts window
	{
    USART_sendstr("interrupt Happened \r\n");
	static unsigned char ucLocalTickCount = 0;
    static signed char xHigherPriorityTaskWoken;

    /* A timer tick has occurred. */

    /*..Do other time functions..*/ 

    /* Is it time for vATask() to run? */
    xHigherPriorityTaskWoken = pdFALSE;
  
        /* Unblock the task by releasing the semaphore. */
    xSemaphoreGiveFromISR( sem, &xHigherPriorityTaskWoken);

    /* If xHigherPriorityTaskWoken was set to true you
    we should yield.  The actual macro used here is
    port specific. */
	
	lastInterrupt = xTaskGetTickCount();
	if(xHigherPriorityTaskWoken)
       vPortYield();
	else
	   USART_sendstr("No Higher Priority Task is Woken \r\n");
	}
}




static void vOddTask( void *pvParameters )
{
  USART_sendstr("Start Odd Task \r\n");
  TickType_t xLastWakeTime = xTaskGetTickCount();
	 
  for( ;; )
    {
       /* Block waiting for the semaphore to become available. */
	   USART_sendstr("Odd \r\n");
        if( xSemaphoreTake(sem, LONG_TIME ) == pdTRUE )
        {
			PORTC = 0x55;
            USART_sendstr("Semaphore is given to Odd Task \r\n");
		    vTaskDelayUntil(&xLastWakeTime, (2000 / portTICK_PERIOD_MS));
		    //if( xSemaphoreGive( sem ) != pdTRUE )
		    	//USART_sendstr("Semaphore can not release by Odd Task \r\n");
			//vTaskDelayUntil(&xLastWakeTime, (100 / portTICK_PERIOD_MS));
        }
			
    }
}


static void vEvenTask( void *pvParameters )
{
	USART_sendstr("Start Even Task \r\n");
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	if( xSemaphoreGive(sem))
	   USART_sendstr("Semaphore is released by Even Task \r\n");
	else
	   USART_sendstr("Semaphore can not release by Even Task \r\n");
	   
	
	for( ;; )
	{
		 USART_sendstr("Even \r\n");
		/* Block waiting for the semaphore to become available. */
		if( xSemaphoreTake(sem, LONG_TIME ) == pdTRUE )
		{
			PORTC = 0xAA;
			USART_sendstr("Semaphore is given to Even Task \r\n");
			
			if( xSemaphoreGive( sem ) != pdTRUE )
				USART_sendstr("Semaphore can not release by Even Task \r\n");
				
			vTaskDelayUntil(&xLastWakeTime, (100 / portTICK_PERIOD_MS));
		}
		
	}
}
