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

MyQueue_Typedef DemoQueue;

uint8_t DemoQueueBuf[1024];

void test3(void)
{
	uint8_t i,j,k,bufx[50];

	for(i=0;i<50;i++)
		bufx[i] = i;

	if(MyQueue_Create(&DemoQueue,DemoQueueBuf,sizeof(DemoQueueBuf)/sizeof(DemoQueueBuf[0]),sizeof(DemoQueueBuf[0])))
	{
		printf("queue data count : %u\r\n",MyQueue_GetCount(&DemoQueue));
		printf("queue data left count : %u\r\n",MyQueue_GetLeftCount(&DemoQueue));
		printf("queue is full  : %u\r\n",MyQueue_IsFull(&DemoQueue));
		printf("queue is empty  : %u\r\n",MyQueue_IsEmpty(&DemoQueue));
		printf("queue size : %u\r\n",MyQueue_GetSize(&DemoQueue));
		printf("\r\n");

		while(1)
		{
			gets(s1);

			if(strncmp(s1,"put",3) == 0)
			{
				j = atoi(s1+3);
				if(j == 1)
				{
					if(MyQueue_Put(&DemoQueue,bufx))
					{
						queue3info();
					}

					else
						printf("put error\r\n");
				}
				else
				{
					if(MyQueue_PutLength(&DemoQueue,bufx,j))
					{
						queue3info();
					}

					else
						printf("put error\r\n");

				}
			}

			else if(strncmp(s1,"get",3) == 0)
			{
				j = atoi(s1+3);
				if(j == 1)
				{
					if(MyQueue_Get(&DemoQueue,bufget))
					{
						printf("bufget[0] : %u\r\n",bufget[0]);
						queue3info();
					}
					else
					{
						printf("get error\r\n");
					}
				}
				else
				{
					if(MyQueue_GetLength(&DemoQueue,bufget,j))
					{
						for(i=0;i<j;i++)
						{
							printf("bufget[%u] : %u\r\n",i,bufget[i]);
						}
						queue3info();
					}
					else
					{
						printf("get error\r\n");
					}
				}
			}

			else if(strncmp(s1,"peek",4) == 0)
			{
				if(MyQueue_Peek(&DemoQueue,bufget))
				{
					printf("bufget[0] : %u\r\n",bufget[0]);
					queue3info();
				}
				else
				{
					printf("peek error\r\n");
				}
			}
			else if(strncmp(s1,"peel",4) == 0)
			{
				j = atoi(s1+4);
				if(MyQueue_PeekLength(&DemoQueue,bufget,j))
				{
					for(i=0;i<j;i++)
					{
						printf("bufget[%u] : %u\r\n",i,bufget[i]);
					}
					queue3info();
				}
				else
				{
					printf("peel error\r\n");
				}
			}
			else if(strncmp(s1,"peeo",4) == 0)
			{
				j = atoi(s1+4);
				if(MyQueue_Peek_Offset(&DemoQueue,bufget,j))
				{
					printf("bufget[%u] : %u\r\n",j,bufget[0]);
					queue3info();
				}
				else
				{
					printf("peeo error\r\n");
				}
			}
		}
	}
	else
	{
		printf("creat error\r\n");
	}
}
