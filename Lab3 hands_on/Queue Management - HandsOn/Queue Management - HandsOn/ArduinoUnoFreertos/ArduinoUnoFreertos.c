#define F_CPU 16000000UL
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "USART.h"
#include "queue.h"

/* Demo includes. */
#include "supporting_functions.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

/* The tasks to be created.  Two instances are created of the sender task while
only a single instance is created of the receiver task. */
static void vSenderTask( void *pvParameters );
static void vReceiverTask( void *pvParameters );

/*-----------------------------------------------------------*/

/* Declare a variable of type QueueHandle_t.  This is used to store the queue
that is accessed by all three tasks. */
QueueHandle_t xQueue;


int main( void )
{
	USART_init();
	
	////////////////////// To Do 1//////////////////////////////
	
    /* The queue is created to hold a maximum of 5 long values. 
    Create A Suitable Queue
     */
	
	if( xQueue != NULL ) // Queue Handler
	{		////////////////////// To Do 2//////////////////////////////

		/* 1. Create two instances of the task that will write to the queue.  The
		parameter is used to pass the value that the task should write to the queue,
		so one task will continuously write 100 to the queue while the other task
		will continuously write 200 to the queue.  Both tasks are created at
		priority 1. */
	

		/*2. Create the task that will read from the queue.  The task is created with
		priority 2, so above the priority of the sender tasks. */
	
		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
	}
	else
	{
		/* The queue could not be created. */
		USART_sendstr("The queue could not be created");
	}

	/* The following line should never be reached because vTaskStartScheduler()
	will only return if there was not enough FreeRTOS heap memory available to
	create the Idle and (if configured) Timer tasks.  Heap management, and
	techniques for trapping heap exhaustion, are described in the book text. */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

static void vSenderTask( void *pvParameters )
{
char * lValueToSend;
BaseType_t xStatus;

	/* Two instances are created of this task so the value that is sent to the
	queue is passed in via the task parameter rather than be hard coded.  This way
	each instance can use a different value.  Cast the parameter to the required
	type. */
	lValueToSend =(char *)pvParameters;

	/* As per most tasks, this task is implemented within an infinite loop. */
		USART_sendstr("sender");
		TickType_t xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		////////////////////// To Do 3//////////////////////////////
		/*
		   Insert back the element in the Queue without waiting for available space.
		*/
		if( xStatus != pdPASS )
		{
			/* We could not write to the queue because it was full – this must
			be an error as the queue should never contain more than one item! */
			USART_sendstr("Could not send to the queue.");
		}
			vTaskDelayUntil(&xLastWakeTime, (100 / portTICK_PERIOD_MS));
	}
}
/*-----------------------------------------------------------*/

static void vReceiverTask( void *pvParameters )
{
/* Declare the variable that will hold the values received from the queue. */
char * lReceivedValue;
BaseType_t xStatus;
const TickType_t xTicksToWait = pdMS_TO_TICKS( 99UL );

	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		///* As this task unblocks immediately that data is written to the queue this
		//call should always find the queue empty. */
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
		   USART_sendstr("Queue should be empty!\r\n");
		}

		////////////////////// To Do 4//////////////////////////////
		/*
		   Receive the next element in the Queue and wait for a xTicksToWait for arriving at least one element.
		*/
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value. */
			USART_sendstr( "Received = ");
			USART_sendstr(lReceivedValue);
			USART_sendstr( "\r\n");
		}
		else
		{
			/* We did not receive anything from the queue even after waiting for 100ms.
			This must be an error as the sending tasks are free running and will be
			continuously writing to the queue. */
			USART_sendstr("Could not receive from the queue.\r\n" );
		}
	}
}






