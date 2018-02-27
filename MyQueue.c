/**
  * @file		MyQueue.c
  * @author 	mgdg
  * @brief		队列驱动文件
  * @version	v1.0
  * @date		2017-09-28
  * @remark		创建可以存放任意元素的环形队列，包含了入队出队等基本操作
  
  *				
  */

#include "MyQueue.h"

/**
  * @brief	创建队列
  * @param	*queue：队列对象
  * @param	*buf：队列缓存数组指针
  * @param	queue_len：队列长度
  * @param	ItemSize：队列单个元素的大小，以字节为单位

  * @return	bool	
  * @remark		
  */
bool MyQueue_Create(MyQueue_Typedef *queue,void *buf,unsigned short int queue_len,unsigned short int ItemSize)
{
	if(queue_len==0 || ItemSize==0 || buf==NULL || queue==NULL)
		return false;

	queue->buffer = buf;
	queue->size = ItemSize;
	queue->len = queue_len;
	queue->front = queue->rear = 0;
	return true;
}

/**
  * @brief	获取队列已存放数据个数
  * @param	*queue：队列对象
  *
  * @return	unsigned short int:队列数据个数	
  * @remark		
  */
unsigned short int  MyQueue_Num(const MyQueue_Typedef *queue)
{
	if(queue == NULL)
		return 0;

	if(queue->front < queue->rear)
		return (queue->front + queue->len - queue->rear);
	else
		return (queue->front - queue->rear);
}

/**
  * @brief	获取队列剩余可存放数据个数
  * @param	*queue：队列对象
  *
  * @return	unsigned short int:队列剩余可存放数据个数	
  * @remark		
  */
unsigned short int MyQueue_LeftNum(const MyQueue_Typedef *queue)
{
	if(queue == NULL)
		return 0;

	if(queue->front < queue->rear)
		return (queue->rear - queue->front) - 1;
	else
		return (queue->len - (queue->front - queue->rear)) - 1;
}


/**
  * @brief	获取队列总容量
  * @param	*queue：队列对象
  *
  * @return	unsigned short int:队列容量
  * @remark		
  */
unsigned short int MyQueue_Size(const MyQueue_Typedef *queue)
{	
	if(queue == NULL)
		return 0;

	return queue->len - 1;
}


/**
  * @brief	队列是否已满
  * @param	*queue：队列对象
  *
  * @return	bool:队列满状态
  * @remark		
  */
bool MyQueue_IsFull(const MyQueue_Typedef *queue)
{
	if(queue == NULL)
		return false;

	return MyQueue_Num(queue)  == ((queue->len)-1);
}


/**
  * @brief	队列是否已空
  * @param	*queue：队列对象
  *
  * @return	bool:队列空状态
  * @remark		
  */
bool MyQueue_IsEmpty(const MyQueue_Typedef *queue)
{
	if(queue == NULL)
		return false;

	return MyQueue_Num(queue) == 0;
}


/**
  * @brief	将指定个数的数据放入队列
  * @param	*queue：队列对象
  * @param	*buf：数据指针
  * @param	num：数据个数
  * 
  * @return	bool
  * @remark	
  */
bool MyQueue_Put(MyQueue_Typedef *queue,const void *buf,unsigned short int num)
{
	char *dst;
	char *src = (char *)buf;
	unsigned short int templen;

	if(queue==NULL || buf==NULL)
		return false;
	
	if( num > MyQueue_LeftNum(queue))
		return false;
	
	while(num)
	{
		dst = ((char *)queue->buffer) + (queue->front)*(queue->size);
		
		templen = (queue->len)-(queue->front);
		
		if(num <= templen)
			templen = num;

		num -= templen;
		
		memcpy(dst,src,templen * (queue->size));

		src += (templen * (queue->size));

		queue->front += templen;
		queue->front %= queue->len;
	}	
	
	return true;
}


/**
  * @brief	从队列中取出指定个数的数据
  * @param	*queue：队列对象
  * @param	*buffer：数据存放指针
  * @param	num：取出的队列个数
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Get(MyQueue_Typedef *queue,void *buf,unsigned short int num)
{
	char *dst = (char *)buf;
	char *src;
	unsigned short int templen;

	if(queue==NULL || buf==NULL)
		return false;
	
	if(num > MyQueue_Num(queue))
		return false;

	while(num)
	{
		src = ((char *)queue->buffer) + (queue->rear)*(queue->size);
		
		templen = (queue->len)-(queue->rear);

		if(num <= templen)
			templen = num;
		
		num -= templen;

		memcpy(dst,src,templen * (queue->size));
		dst += (templen * (queue->size));
		queue->rear += templen;
		queue->rear %= queue->len;
	}
	
	return true;
}



/**
  * @brief	从指定偏移位置开始，从队列中获取指定个数的数据，但是不弹出数据
  * @param	*queue：队列对象
  * @param	*buf
  * @param	num：数据长度
  * @param	offset：偏移位置
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Peek(const MyQueue_Typedef *queue,void *buf,unsigned short int num,unsigned short int offset)
{
	char *dst = (char *)buf;
	char *src;
	unsigned short int templen;
	unsigned short int temprear;
	
	if(queue==NULL || buf==NULL)
		return false;

	if((offset+num) > (MyQueue_Num(queue)))
		return false;
	
	temprear = (queue->rear)+offset;
	temprear %= queue->len; 
	
	while(num)
	{
		src = ((char *)queue->buffer) + (temprear)*(queue->size);
		
		templen = (queue->len)-(temprear);

		if(num <= templen)
			templen = num;

		num -= templen;

		memcpy(dst,src,templen * (queue->size));
		dst += (templen * (queue->size));
		temprear += templen;
		temprear %= queue->len;
	}

	return true;
}



/**
  * @brief	弹掉指定个数的数据，不使用数据
  * @param	*queue：队列对象
  * @param	num：长度
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Pop(MyQueue_Typedef *queue,unsigned short int num)
{
	if(queue==NULL)
		return false;

	if(num <= MyQueue_Num(queue))
	{	
		queue->rear+=num;
		queue->rear%=queue->len;   
		return true;
	}
	else
		return false;
}


/**
  * @brief	POP所有数据，不使用数据
  * @param	*queue：队列对象
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_PopAll(MyQueue_Typedef *queue)
{
	if(queue==NULL)
		return false;
	else
	{
		queue->rear = queue->front;
		return true;
	}
}
