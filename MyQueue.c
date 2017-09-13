/**
  ******************************************************************************
  * @file    MyQueue.c
  * @author  mgdg
  * @version V1.0.0
  * @date    2017-09-13
  * @brief   
  ******************************************************************************
 **/


#include "MyQueue.h"


/**
  * @brief	初始化队列
  * @param	*queue：队列对象
  * @param	*buffer：队列数组首地址
  * @param	max_size：队列容量

  * @return	void	
  * @remark		
  */
void queue_init_static(Queue_Typedef *queue, uint8_t *buffer, uint16_t max_size) 
{
    queue->buffer = buffer;
    queue->max_size = max_size;
    queue->front = queue->rear = 0;
}

/**
  * @brief	计算队列数据个数
  * @param	*QUEUE：队列对象
  *
  * @return	uint16_t:队列数据个数	
  * @remark		
  */
uint16_t  queue_GetCount(Queue_Typedef *queue)
{
	if(queue->front < queue->rear)
		return queue->front + queue->max_size - queue->rear;
	else
		return queue->front - queue->rear;
}

/**
  * @brief	计算队列剩余数据个数
  * @param	*QUEUE：队列对象
  *
  * @return	uint16_t:队列剩余数据个数	
  * @remark		
  */
uint16_t queue_GetLeftCount(Queue_Typedef *queue)
{
	if(queue->front < queue->rear)
		return (queue->rear - queue->front);
	else
		return (queue->max_size - (queue->front - queue->rear));
}

/**
  * @brief	检测队列满
  * @param	*QUEUE：队列对象
  *
  * @return	bool:队列满状态
  * @remark		
  */
bool queue_IsFull(Queue_Typedef *queue)
{
	return queue_GetCount(queue)  == (queue->max_size - 1);
}

/**
  * @brief	检测队列空
  * @param	*QUEUE：队列对象
  *
  * @return	bool:队列空状态
  * @remark		
  */
bool queue_IsEmpty(Queue_Typedef *queue)
{
	return queue_GetCount(queue) == 0;
};

/**
  * @brief	获取队列容量
  * @param	*QUEUE：队列对象
  *
  * @return	uint16_t:队列容量
  * @remark		
  */
uint16_t queue_GetQueSize(Queue_Typedef *queue)
{	
	return queue->max_size;
};

/**
  * @brief	将队列成员放入队列
  * @param	*QUEUE：队列对象
  * @param	ch：数据
  *
  * @return	bool:运行状态
  * @remark		
  */
bool queue_Put(Queue_Typedef *queue,uint8_t ch)
{
  	if(!queue_IsFull(queue))
  	{
    	queue->buffer[queue->front]= ch;
    	queue->front ++;
    	queue->front %= queue->max_size; 
    	return true;
  	}
  	else
    	return false;
}

/**
  * @brief	将指定长度成员放入队列
  * @param	*QUEUE：队列对象
  * @param	buf：数据
  * @param	len：长度
  * @return	bool:运行状态
  * @remark	有bug，队列刚好存满的时候会导致队列为空，所以队列不能存满	
  */
bool queue_PutLength(Queue_Typedef *queue,const uint8_t *buf,uint16_t len)
{
	uint16_t i;
	if(len < queue_GetLeftCount(queue))
	{
		for(i=0;i<len;i++)
		{
			queue->buffer[queue->front]= *(buf++);
    		queue->front ++;
    		queue->front %= queue->max_size;
		}
		return true;
	}
	else
		return false;
}

/**
  * @brief	将队列成员取出队列
  * @param	*QUEUE：队列对象
  * @param	ch：数据
  * @return	bool:运行状态
  * @remark		
  */
bool queue_Get(Queue_Typedef *queue,uint8_t *ch)
{
  	if (queue_GetCount(queue))
  	{
    	*ch = queue->buffer[queue->rear];
    	queue->rear++;
    	queue->rear%=queue->max_size;   
    	return true;
  	}
  	else
    	return false;
}

/**
  * @brief	获取数据，但是不弹出数据
  * @param	*QUEUE：队列对象
  * @param	ch：数据
  * @return	bool:运行状态
  * @remark		
  */
bool queue_Peek(Queue_Typedef *queue,uint8_t *ch)
{
  	if (queue_GetCount(queue))
  	{
    	*ch = queue->buffer[queue->rear];
    	return true;
  	}
  	else
    	return false;
}

/**
  * @brief	获取指定长度数据，但是不弹出数据
  * @param	*QUEUE：队列对象
  * @param	*buffer：数据首地址
  * @param	length：数据长度
  * @return	bool:运行状态
  * @remark		
  */
bool queue_PeekLength(Queue_Typedef *queue,uint8_t *buffer,uint16_t length)
{
	uint16_t i;
	
	if(length > queue_GetCount(queue))
		return false;
	
	for (i = 0; i < length; ++i)
	{
		*(buffer++) = queue->buffer[(queue->rear + i) % queue->max_size];
	}

	return true;
}

/**
  * @brief	获取所有数据，但是不弹出数据
  * @param	*QUEUE：队列对象
  * @param	*buffer：数据首地址
  * @return	bool:运行状态
  * @remark	输出缓存空间小于队列时将导致错误	
  */
bool queue_PeekAll(Queue_Typedef *queue,uint8_t *buffer)
{
	uint16_t back_tail;

	if( !buffer ) return false;

	back_tail = queue->rear;
	
	while (back_tail != queue->front)
	{
		*(buffer++) = queue->buffer[back_tail++];
		back_tail %= queue->max_size;
	}

	return true;
}

/**
  * @brief	POP一个数据，不使用数据
  * @param	*QUEUE：队列对象
  * @return	bool:运行状态
  * @remark		
  */
bool queue_Pop(Queue_Typedef *queue)
{
  	if (queue_GetCount(queue))
  	{
    	queue->rear++;
    	queue->rear%=queue->max_size;   
		return true;
  	}
	else
		return false;
}

/**
  * @brief	获取队列中指定偏移位置的一个数据
  * @param	*QUEUE：队列对象
  * @param	offset：偏移
  * 
  * @return	bool:运行状态
  * @remark		
  */
bool queue_Peek_Offset(Queue_Typedef *queue,uint8_t *ch,uint16_t offset)
{
	uint16_t temp;

	if((offset+1) > (queue_GetCount(queue)))
		return false;
	
	temp = queue->rear+offset;
	temp %= queue->max_size; 
	*ch = queue->buffer[temp];
	return true;
}

//取出指定长度，并弹出
/**
  * @brief	取出指定长度，并弹出
  * @param	*QUEUE：队列对象
  * @param	*buffer：数据首地址
  * @param	length：长度
  * 
  * @return	bool:运行状态
  * @remark		
  */
bool queue_PeekPopLength(Queue_Typedef *queue,uint8_t *buffer,uint16_t length)
{
	uint16_t i;
	
	if(length > queue_GetCount(queue))
		return false;
	
	for (i = 0; i < length; ++i)
	{
		*(buffer++) = queue->buffer[(queue->rear) % queue->max_size];
		queue->rear++;
		queue->rear%=queue->max_size; 

	}
	return true;
}

/**
  * @brief	POP指定长的数据，不使用数据
  * @param	*QUEUE：队列对象
  * @param	length：长度
  * 
  * @return	bool:运行状态
  * @remark		
  */
bool queue_PopLength(Queue_Typedef *queue,uint16_t length)
{
	if(length <= queue_GetCount(queue))
  	{
    	queue->rear+=length;
    	queue->rear%=queue->max_size;   
		return true;
  	}
	else
		return false;
}

/**
  * @brief	POP所有数据，不使用数据
  * @param	*QUEUE：队列对象
  * @return	void
  * @remark		
  */
void queue_PopAll(Queue_Typedef *queue)
{
	queue->rear = queue->front;
}
  


//demo
#if 0 

#define DEMO_QUEUE_MAX_SIZE	128
static queue_Typedef DemoQueue;			//状态机事件队列
static uint8_t queuebuf[DEMO_QUEUE_MAX_SIZE];

//初始化队列
queue_init_static(&DemoQueue,queuebuf,DEMO_QUEUE_MAX_SIZE);

//将数据放入队列
bool put_in_queue(const uint8_t* pBuf,uint16_t len)
{
	if( !pBuf || len <1 ) 
		return false;

	if(queue_PutLength(&DemoQueue,pBuf,len))
		return true;
	else
		return false;
}

#endif