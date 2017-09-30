/**
  * @file		MyQueue.h
  * @author		mgdg
  * @brief		队列驱动文件
  * @version	v1.0
  * @date		2017-09-28
  * @remark
  */

#ifndef __MYQUEUE_H
#define __MYQUEUE_H


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct{
	void *buffer;			// 数据缓冲区
	uint16_t len;			// 队列长度
	uint16_t front;			// 队列头,指向下一个空闲存放地址
	uint16_t rear;			// 队列尾，指向第一个数据
	uint8_t size;			// 单个数据大小(单位 字节)
	
}MyQueue_Typedef;


bool MyQueue_Create(MyQueue_Typedef *queue,void *buf,uint16_t queue_len,uint8_t queue_size_byte);

uint16_t MyQueue_GetCount(MyQueue_Typedef *queue);
uint16_t MyQueue_GetLeftCount(MyQueue_Typedef *queue);
uint16_t MyQueue_GetSize(MyQueue_Typedef *queue);

bool MyQueue_IsFull(MyQueue_Typedef *queue);
bool MyQueue_IsEmpty(MyQueue_Typedef *queue);

bool MyQueue_Put(MyQueue_Typedef *queue,void *buf);
bool MyQueue_PutLength(MyQueue_Typedef *queue,const void *buf,uint16_t num);

bool MyQueue_Get(MyQueue_Typedef *queue,void *buf);
bool MyQueue_GetLength(MyQueue_Typedef *queue,void *buf,uint16_t num);

bool MyQueue_Peek(MyQueue_Typedef *queue,void *buf);
bool MyQueue_PeekLength(MyQueue_Typedef *queue,void *buf,uint16_t num);
bool MyQueue_Peek_Offset(MyQueue_Typedef *queue,void *buf,uint16_t offset);

bool MyQueue_Pop(MyQueue_Typedef *queue);
bool MyQueue_PopLength(MyQueue_Typedef *queue,uint16_t num);
bool MyQueue_PopAll(MyQueue_Typedef *queue);


#endif
