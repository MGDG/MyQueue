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
  * @param	*buf：队列缓存数组指针
  * @param	queue_len：队列长度
  * @param	queue_size_byte：队列单个元素的大小，以字节为单位

  * @return	bool	
  * @remark		
  */
bool MyQueue_Create(MyQueue_Typedef *queue,void *buf,size_t queue_len,size_t queue_size_byte)
{
	if(queue_len==0 || queue_size_byte==0 || buf==NULL || queue==NULL)
		return false;

	queue->buffer = buf;
	queue->size = queue_size_byte;
	queue->len = queue_len;
	queue->front = queue->rear = 0;
	return true;
}

/**
  * @brief	获取队列已存放数据个数
  * @param	*queue：队列对象
  *
  * @return	size_t:队列数据个数	
  * @remark		
  */
size_t  MyQueue_GetCount(MyQueue_Typedef *queue)
{
	if(queue==NULL)
		return 0;

	else if(queue->front < queue->rear)
		return (queue->front + queue->len - queue->rear);
	else
		return (queue->front - queue->rear);
}

/**
  * @brief	获取队列剩余可存放数据个数
  * @param	*queue：队列对象
  *
  * @return	size_t:队列剩余可存放数据个数	
  * @remark		
  */
size_t MyQueue_GetLeftCount(MyQueue_Typedef *queue)
{
	if(queue==NULL)
		return 0;

	if(queue->front < queue->rear)
		return (queue->rear - queue->front) - 1;
	else
	{
		if(queue->front == queue->rear)
			return queue->len - 1;
		else
			return (queue->len - (queue->front - queue->rear)) - 1;
	}
}


/**
  * @brief	获取队列总容量
  * @param	*queue：队列对象
  *
  * @return	size_t:队列容量
  * @remark		
  */
size_t MyQueue_GetSize(MyQueue_Typedef *queue)
{	
	if(queue==NULL)
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
bool MyQueue_IsFull(MyQueue_Typedef *queue)
{
	if(queue==NULL)
		return false;

	return MyQueue_GetCount(queue)  == ((queue->len)-1);
}


/**
  * @brief	队列是否已空
  * @param	*queue：队列对象
  *
  * @return	bool:队列空状态
  * @remark		
  */
bool MyQueue_IsEmpty(MyQueue_Typedef *queue)
{
	if(queue==NULL)
		return false;

	return MyQueue_GetCount(queue) == 0;
}



/**
  * @brief	将单个数据放入队列
  * @param	*queue：队列对象
  * @param	*ch：数据指针
  *
  * @return	bool
  * @remark		
  */
/*
bool MyQueue_Put(MyQueue_Typedef *queue,const void *buf)
{
	char *dst;

	if(queue==NULL || buf==NULL)
		return false;

  	if(!MyQueue_IsFull(queue))
  	{
		dst = ((char *)queue->buffer)+((queue->front)*(queue->size));
		memcpy(dst,(char *)buf,queue->size);

  		queue->front ++;
		queue->front %= queue->len;
		return true;
  	}
  	else
    	return false;
}
*/

/**
  * @brief	将指定个数的数据放入队列
  * @param	*queue：队列对象
  * @param	*buf：数据指针
  * @param	num：数据个数
  * 
  * @return	bool
  * @remark	
  */
bool MyQueue_Put(MyQueue_Typedef *queue,const void *buf,size_t num)
{
	char *dst;
	char *src = (char *)buf;
	size_t templen;

	if(queue==NULL || buf==NULL)
		return false;
	
	if( num > MyQueue_GetLeftCount(queue))
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
  * @brief	从队列中取出一个数据
  * @param	*queue：队列对象
  * @param	*ch：数据存放指针
  * 
  * @return	bool
  * @remark		
  */
/*
bool MyQueue_Get(MyQueue_Typedef *queue,void *buf)
{
	char *dst;
	char *src;

	if(queue==NULL || buf==NULL)
		return false;

  	if (MyQueue_GetCount(queue))
  	{
  		dst = (char *)buf;
		src = ((char *)queue->buffer) + (queue->rear)*(queue->size);
  		memcpy(dst,src,queue->size);

    	queue->rear++;
    	queue->rear%=queue->len;   
    	return true;
  	}
  	else
    	return false;
}
*/

/**
  * @brief	从队列中取出指定个数的数据
  * @param	*queue：队列对象
  * @param	*buffer：数据存放指针
  * @param	num：取出的队列个数
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Get(MyQueue_Typedef *queue,void *buf,size_t num)
{
	char *dst = (char *)buf;
	char *src;
	size_t templen;

	if(queue==NULL || buf==NULL)
		return false;
	
	if(num > MyQueue_GetCount(queue))
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
  * @brief	从队列中取出一个数据但不弹掉
  * @param	*queue：队列对象
  * @param	*ch：数据存放指针
  * 
  * @return	bool
  * @remark		
  */
/*
bool MyQueue_Peek(MyQueue_Typedef *queue,void *buf)
{
	char *dst;
	char *src;

	if(queue==NULL || buf==NULL)
		return false;

  	if (MyQueue_GetCount(queue))
  	{
  		dst = (char *)buf;
		src = ((char *)queue->buffer) + (queue->rear)*(queue->size);
  		memcpy(dst,src,queue->size);
    	return true;
  	}
  	else
    	return false;
}
*/

/**
  * @brief	从队列中获取指定个数的数据，但是不弹出数据
  * @param	*queue：队列对象
  * @param	*buffer：数据存放首地址
  * @param	num：数据长度
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Peek(MyQueue_Typedef *queue,void *buf,size_t num)
{
	size_t temprear;
	char *dst = (char *)buf;
	char *src;
	size_t templen;

	if(queue==NULL || buf==NULL)
		return false;
	
	if(num > MyQueue_GetCount(queue))
		return false;

	temprear = queue->rear;

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
  * @brief	获取指定偏移位置的一个数据，不弹出
  * @param	*queue：队列对象
  * @param	*ch：数据存放首地址
  * @param	offset：偏移
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Peek_Offset(MyQueue_Typedef *queue,void *buf,size_t offset)
{
	size_t temp;
	char *dst;
	char *src;

	if(queue==NULL || buf==NULL)
		return false;

	if((offset+1) > (MyQueue_GetCount(queue)))
		return false;
	
	temp = (queue->rear)+offset;
	temp %= queue->len; 
	src = ((char *)queue->buffer) + (temp)*(queue->size);

	dst = (char *)buf;
	
	memcpy(dst,src,queue->size);

	return true;
}


/**
  * @brief	弹掉一个数据，不使用数据
  * @param	*queue：队列对象
  * 
  * @return	bool
  * @remark		
  */
/*
bool MyQueue_Pop(MyQueue_Typedef *queue)
{
	if(queue==NULL)
		return false;

  	if (MyQueue_GetCount(queue))
  	{
    	queue->rear++;
    	queue->rear%=queue->len;   
		return true;
  	}
	else
		return false;
}
*/


/**
  * @brief	弹掉指定个数的数据，不使用数据
  * @param	*queue：队列对象
  * @param	num：长度
  * 
  * @return	bool
  * @remark		
  */
bool MyQueue_Pop(MyQueue_Typedef *queue,size_t num)
{
	if(queue==NULL)
		return false;

	if(num <= MyQueue_GetCount(queue))
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
