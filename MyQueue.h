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
	size_t len;				// 队列长度
	size_t front;			// 数据头,指向下一个空闲存放地址
	size_t rear;			// 数据尾，指向第一个数据
	size_t size;			// 单个数据大小(单位 字节)
	
}MyQueue_Typedef;

/**
  * @brief	创建队列
  * @param	*buf：队列缓存数组指针
  * @param	queue_len：队列长度
  * @param	queue_size_byte：队列单个元素的大小，以字节为单位

  * @return	bool	
  * @remark		
  */
bool MyQueue_Create(MyQueue_Typedef *queue,void *buf,size_t queue_len,size_t queue_size_byte);

/**
  * @brief	获取队列已存放数据个数
  * @param	*queue：队列对象
  *
  * @return	size_t:队列数据个数	
  * @remark		
  */
size_t MyQueue_GetCount(MyQueue_Typedef *queue);

/**
  * @brief	获取队列剩余可存放数据个数
  * @param	*queue：队列对象
  *
  * @return	size_t:队列剩余可存放数据个数	
  * @remark		
  */
size_t MyQueue_GetLeftCount(MyQueue_Typedef *queue);

/**
  * @brief	获取队列总容量
  * @param	*queue：队列对象
  *
  * @return	size_t:队列容量
  * @remark		
  */
size_t MyQueue_GetSize(MyQueue_Typedef *queue);

/**
  * @brief	队列是否已满
  * @param	*queue：队列对象
  *
  * @return	bool:队列满状态
  * @remark		
  */
bool MyQueue_IsFull(MyQueue_Typedef *queue);

/**
  * @brief	队列是否已空
  * @param	*queue：队列对象
  *
  * @return	bool:队列空状态
  * @remark		
  */
bool MyQueue_IsEmpty(MyQueue_Typedef *queue);

/**
  * @brief	将指定个数的数据放入队列
  * @param	*queue：队列对象
  * @param	*buf：数据指针
  * @param	num：数据个数
  * 
  * @return	bool
  * @remark	
  */
bool MyQueue_Put(MyQueue_Typedef *queue,const void *buf,size_t num);

/**
  * @brief	从队列中取出指定个数的数据
  * @param	*queue：队列对象
  * @param	*buffer：数据存放指针
  * @param	num：取出的队列个数
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Get(MyQueue_Typedef *queue,void *buf,size_t num);

/**
  * @brief	从队列中获取指定个数的数据，但是不弹出数据
  * @param	*queue：队列对象
  * @param	*buffer：数据存放首地址
  * @param	num：数据长度
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Peek(MyQueue_Typedef *queue,void *buf,size_t num);

/**
  * @brief	获取指定偏移位置的一个数据，不弹出
  * @param	*queue：队列对象
  * @param	*ch：数据存放首地址
  * @param	offset：偏移
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Peek_Offset(MyQueue_Typedef *queue,void *buf,size_t offset);

/**
  * @brief	弹掉指定个数的数据，不使用数据
  * @param	*queue：队列对象
  * @param	num：长度
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Pop(MyQueue_Typedef *queue,size_t num);

/**
  * @brief	POP所有数据，不使用数据
  * @param	*queue：队列对象
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_PopAll(MyQueue_Typedef *queue);


#endif
