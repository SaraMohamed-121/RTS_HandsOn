#define F_CPU 16000000UL
#define TrackSize 5

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "USART.h"
#include "queue.h"

//#include <Arduino.h> //Interrupt
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

static void vGetNextTrack();
static void vReceiverTask(void *pvParameters );

/*-----------------------------------------------------------*/

volatile unsigned long lastInterrupt;

volatile int playListIndex = 0;
char const *playList[] = {"Track1", "Track2", "Track3", "Track4", "Track5"};
	
QueueHandle_t xQueue;

//-----------------------------------------------------------

int main(void)
{ 
	// todo #1 
	// 1. disable global interrupt
	// 2. assign vGetNextTrack method to external Int0
	cli();
	attachIntruppt(0,vGetNextTrack,1);
	USART_init();
	//sei();
    /* The queue is created to hold a maximum of 5 long values. */
    xQueue = xQueueCreate(5, sizeof(char *));
	if( xQueue != NULL )
	{
		xTaskCreate(vReceiverTask, "Receiver", 256, NULL, 2, NULL );
		sei();
		vTaskStartScheduler();
	}
	else
		USART_sendstr("The queue could not be created");

	for( ;; );
	return 0;
	
}


static void vGetNextTrack()
{
  char *lValueToSend = NULL;
  BaseType_t xStatus;
  
  if(xTaskGetTickCountFromISR() - lastInterrupt > 500) // we set a 10ms no-interrupts window
  {
	// todo #2
 	// 1. set lValueToSend to the right track
	 lValueToSend=playList[playListIndex];
 	// 2. send lValueToSend to Queue using appropriate methods 'suffix by FromISR'
    xQueueSendToBackFromISR(xQueue,lValueToSend,0);
	
	if( xStatus != pdPASS )
      USART_sendstr("Could not send to the queue.");
	 		
    lastInterrupt = xTaskGetTickCountFromISR();
	
    playListIndex++;
	if(playListIndex >= TrackSize)
	  playListIndex = 0;
  }
}


static void vReceiverTask( void *pvParameters )
{
/* Declare the variable that will hold the values received from the queue. */
    char * lReceivedValue;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS( 99UL );
   	TickType_t xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
			USART_sendstr("Queue should have been empty!\r\n");
			
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
		if( xStatus == pdPASS )
		{
			USART_sendstr("Received= system runs ");
			USART_sendstr(lReceivedValue);
			USART_sendstr( "\r\n");
		}
		else
			USART_sendstr("Could not receive from the queue.\r\n" );
		
		vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_PERIOD_MS));
	}
}
