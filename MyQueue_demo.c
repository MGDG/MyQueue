/**
  * @file		MyQueue_demo.c
  * @author		mgdg
  * @brief		demo
  * @version	v1.0
  * @date		2017-09-28
  * @remark
  */

#include <stdlib.h>
#include <stdio.h>
#include "stdbool.h"
#include "MyQueue.h"

typedef unsigned          char uint8_t;
typedef unsigned           int uint32_t;

myQueueHandle_t DemoQueue;

uint8_t DemoQueueBuf[1024];

void test3(void)
{
	uint8_t bufx[50],bufy[50];
	uint32_t i,num;

	for(i=0;i<50;i++)
		bufx[i] = i;

	memset(DemoQueueBuf,0x00,sizeof(DemoQueueBuf));
	memset(bufy,0x00,sizeof(bufy));
	
	DemoQueue = myQueueCreate(1024,1);

	if(NULL != DemoQueue)
	{

		printf("queue data count : %d\r\n",myQueueNum(DemoQueue));
		printf("queue data left count : %d\r\n",myQueueLeftNum(DemoQueue));
		printf("queue is full  : %d\r\n",myQueueIsFull(DemoQueue));
		printf("queue is empty  : %d\r\n",myQueueIsEmpty(DemoQueue));
		printf("queue size : %d\r\n",myQueueCapacity(DemoQueue));
		printf("creat success\r\n\r\n");


		myQueuePut(DemoQueue,bufx,10);
		printf("queue data count : %d\r\n",myQueueNum(DemoQueue));
		printf("queue data left count : %d\r\n",myQueueLeftNum(DemoQueue));
		printf("queue is full  : %d\r\n",myQueueIsFull(DemoQueue));
		printf("queue is empty  : %d\r\n",myQueueIsEmpty(DemoQueue));
		printf("queue size : %d\r\n",myQueueCapacity(DemoQueue));

		printf("\r\n");

		for(i=0;i<10;i++)
			printf("%02X ",bufy[i]);
		printf("\r\n");

		myQueuePeek(DemoQueue,bufy,5,5);
		for(i=0;i<10;i++)
			printf("%02X ",bufy[i]);
		printf("\r\n");
		printf("queue data count : %d\r\n",myQueueNum(DemoQueue));
		printf("queue data left count : %d\r\n",myQueueLeftNum(DemoQueue));
		printf("queue is full  : %d\r\n",myQueueIsFull(DemoQueue));
		printf("queue is empty  : %d\r\n",myQueueIsEmpty(DemoQueue));
		printf("queue size : %d\r\n",myQueueCapacity(DemoQueue));
		
	}
	else
	{
		printf("creat error\r\n");
	}
}

void main(void)
{
	test3();
	while(1);
}