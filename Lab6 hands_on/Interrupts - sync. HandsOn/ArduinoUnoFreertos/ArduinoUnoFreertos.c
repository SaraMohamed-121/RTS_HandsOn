#define F_CPU 16000000UL
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

void* producer(void* arg);
void* consumer(void* arg);

#define LONG_TIME 0xffff

//-----------------------------------------------------------
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
// Shared resources
#define BUFFER_SIZE 10
int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer

SemaphoreHandle_t sem;
int main(void)
{ 
  cli();
 
  USART_init();
  sem =  xSemaphoreCreateBinary();
  if(sem != NULL){
    DDRD = 0xff;
	xTaskCreate( producer, "producer", 256, NULL, 1, &TaskHandle_1 );
	xTaskCreate( consumer, "consumer", 256, NULL, 3, &TaskHandle_2 );
	sei();
	vTaskStartScheduler();
	}
  else
	   USART_sendstr("Semaphore not Created Correctly !!");
	 
	for( ;; );
	return 0;
	
	
}



void* producer(void* arg) {
	while (1) {

	int item = rand() % 100; // Generate a random number as an item 
 if(  xSemaphoreTake(sem,portMAX_DELAY)==pdTRUE /* TODO 5 call an API to get/take the semaphore*/)
        {
		 // TO DO 2  // Produce item (fill the buffer with the item ) then increment the count
           	if (count<10)
           	{
	           	buffer[count]=item;
	           	count++;
           	}
		   //TO DO 3  //print ("Produced: %d\n", item);
       printf("Consumed: %d\n", item);
       USART_sendstr(item);

        // Exit critical section
        	if( xSemaphoreGive(sem)!=pdTRUE ) //TO DO 4 IF releasing the semaphore"" != pdTRUE 
				USART_sendstr("Semaphore can not release by PRODUCER Task \r\n");
   

        // Simulate work
       // sleep(1);
    }
 }
    return NULL;

}
void* consumer(void* arg) {
    while (1) {
         if( xSemaphoreTake(sem,portMAX_DELAY)==pdTRUE /* TODO 5 call an API to get/take the semaphore*/)
			 {

        // Consume item
        int item = buffer[count - 1];
        count--;
	 //TO DO 6 //print ("Consumed: %d\n", item);
       printf("Consumed: %d\n", item);
       USART_sendstr(item);
        

      
        if( xSemaphoreGive(sem)!=pdTRUE) //TO DO 7 IF releasing the semaphore"" != pdTRUE 
			USART_sendstr("Semaphore can not release by PRODUCER Task \r\n");
  

        // Simulate work
        //sleep(2);
    }
    return NULL;
}
}



