/**
  * @file		MyQueue.c
  * @author 	mgdg
  * @brief		队列驱动文件
  * @version	v1.0
  * @date		2017-09-28
  * @remark		创建可以存放任意元素的环形队列，包含了入队出队等基本操作			
  */

/*只有一个生产者和消费者的情况下不需要锁保护*/
//#define MYQUEUE_USE_LOCK

#include "MyQueue.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef MYQUEUE_USE_LOCK
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define	MYQUEUE_API_CREATELOCK(v)		do{(v)->lock = xSemaphoreCreateMutex();assert((v)->lock);}while(0);
#define MYQUEUE_API_DELETELOCK(v)		do{vSemaphoreDelete((v)->lock);}while(0);
#define	MYQUEUE_API_LOCK(v)				do{xSemaphoreTake((v)->lock, portMAX_DELAY);}while(0);
#define MYQUEUE_API_UNLOCK(v)			do{xSemaphoreGive((v)->lock);}while(0);
#else
#define	MYQUEUE_API_CREATELOCK(v)
#define MYQUEUE_API_DELETELOCK(v)
#define	MYQUEUE_API_LOCK(v)
#define MYQUEUE_API_UNLOCK(v)
#endif

#define debug_i(format,...)				/*printf(format"\n",##__VA_ARGS__)*/
#define NUM_IN_QUEUE(v)					((((v)->front) <= ((v)->rear))?(((v)->rear)-((v)->front)):(((v)->len)-((v)->front)+((v)->rear)))
#define LFET_NUM_IN_QUEUE(v)			((((v)->front) <= ((v)->rear))?((((v)->len)-1)-(((v)->rear)-((v)->front))):(((v)->front)-((v)->rear)-1))

typedef struct {
	void                *buffer;        /*数据缓冲区*/
	size_t              len;            /*队列长度*/
	size_t              size;           /*单个数据大小(单位 字节)*/
	size_t              front;          /*数据头,指向下一个空闲存放地址*/
	size_t              rear;           /*数据尾，指向第一个数据*/
#ifdef MYQUEUE_USE_LOCK
	SemaphoreHandle_t   lock;           /*保护锁*/
#endif
}MYQUEUE_T;

myQueueHandle_t myQueueCreate(size_t queue_len,size_t item_size)
{
	if((0==queue_len) || (0==item_size)) {
		debug_i("create queue,len=%d,ItemSize=%d",queue_len,item_size);
		return NULL;
	}
	MYQUEUE_T *queue = calloc(1,sizeof(MYQUEUE_T));
	assert(queue);
	queue->size = item_size;
	queue->len = queue_len;
	queue->buffer = malloc((queue->size) * (queue->len));
	assert(queue->buffer);
	queue->front = queue->rear = 0;
	MYQUEUE_API_CREATELOCK(queue);
	MYQUEUE_API_UNLOCK(queue);
	return (myQueueHandle_t)queue;
}

void myQueueDelete(myQueueHandle_t queue)
{
	if(NULL == queue) {
		return;
	}
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	MYQUEUE_API_LOCK(p);
	free(p->buffer);
	MYQUEUE_API_UNLOCK(p);
	MYQUEUE_API_DELETELOCK(p);
	free(queue);
}

size_t  myQueueNum(const myQueueHandle_t queue)
{
	if(NULL == queue) {
		return 0;
	}
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	MYQUEUE_API_LOCK(p);
	size_t num_in_queue = NUM_IN_QUEUE(p);
	MYQUEUE_API_UNLOCK(p);
	return num_in_queue;
}

size_t myQueueLeftNum(const myQueueHandle_t queue)
{
	if(NULL == queue) {
		return 0;
	}
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	MYQUEUE_API_LOCK(p);
	size_t left_num_in_queue = LFET_NUM_IN_QUEUE(p);
	MYQUEUE_API_UNLOCK(p);
	return left_num_in_queue;
}

size_t myQueueCapacity(const myQueueHandle_t queue)
{
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	return (NULL==p)?0:(p->len - 1);
}

bool myQueueIsFull(const myQueueHandle_t queue)
{
	if(NULL == queue) {
		return false;
	}
	return (myQueueNum(queue)  == ((((MYQUEUE_T *)queue)->len)-1));
}

bool myQueueIsEmpty(const myQueueHandle_t queue)
{
	return (NULL==queue)?false:(0==myQueueNum(queue));
}

bool myQueuePut(myQueueHandle_t queue,const void *buf,size_t num)
{
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	if((NULL==p) || (NULL==p->buffer) || (NULL==buf) || (0==num)) {
		debug_i("put queue=%p,buf=%p,num=%d",queue,buf,num);
		return false;
	}
	bool rt = true;
	MYQUEUE_API_LOCK(p);
	size_t left_num_in_queue = LFET_NUM_IN_QUEUE(p);
	if( num <= left_num_in_queue) {
		char *src = (char *)buf;
		while(num) {
			char *dst = ((char *)(p->buffer)) + (p->rear)*(p->size);
			size_t templen = (p->len)-(p->rear);
			if(num <= templen) {
				templen = num;
			}
			num -= templen;
			size_t copy_len = templen * (p->size);
			memcpy(dst,src,copy_len);
			src += copy_len;
			p->rear = (p->rear + templen) % (p->len);
		}
	}
	else {
		rt = false;
		debug_i("put failed, num=%d,left=%d,front=%d,rear=%d,len=%d",num,left_num_in_queue,p->front,p->rear,p->len);
	}
	MYQUEUE_API_UNLOCK(p);
	return rt;
}

bool myQueueGet(myQueueHandle_t queue,void *buf,size_t num)
{
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	if((NULL==p) || (NULL==p->buffer) || (NULL==buf) || (0==num)) {
		debug_i("get queue=%p,buf=%p,num=%d",queue,buf,num);
		return false;
	}
	bool rt = true;
	MYQUEUE_API_LOCK(p);
	size_t num_in_queue = NUM_IN_QUEUE(p);
	if(num <= num_in_queue) {
		char *dst = (char *)buf;
		while(num) {
			char *src = ((char *)(p->buffer)) + (p->front)*(p->size);
			size_t templen = (p->len)-(p->front);
			if(num <= templen) {
				templen = num;
			}
			num -= templen;
			size_t copy_len = templen * (p->size);
			memcpy(dst,src,copy_len);
			dst += copy_len;
			p->front = (p->front + templen) % (p->len);
		}
	}
	else {
		rt = false;
		debug_i("get failed, num=%d,inQueue=%d,front=%d,rear=%d,len=%d",num,num_in_queue,p->front,p->rear,p->len);
	}
	MYQUEUE_API_UNLOCK(p);
	return rt;
}

bool myQueuePeek(const myQueueHandle_t queue,void *buf,size_t num,size_t offset)
{
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	if((NULL==p) || (NULL==p->buffer) || (NULL==buf) || (0==num)) {
		debug_i("peek queue=%p,buf=%p,num=%d",p,buf,num);
		return false;
	}
	bool rt = true;
	MYQUEUE_API_LOCK(p);
	size_t num_in_queue = NUM_IN_QUEUE(p);
	if((offset+num) <= num_in_queue) {
		char *dst = (char *)buf;
		size_t temp_front = (p->front)+offset;
		while(num) {
			char *src = ((char *)(p->buffer)) + temp_front*(p->size);
			size_t templen = (p->len)-temp_front;
			if(num <= templen) {
				templen = num;
			}
			num -= templen;
			size_t copy_len = templen * (p->size);
			memcpy(dst,src,copy_len);
			dst += copy_len;
			temp_front = (temp_front + templen) % (p->len);
		}
	}
	else {
		rt = false;
		debug_i("peek failed, num=%d,offst=%d,inQueue=%d,front=%d,rear=%d,len=%d",\
				num,offset,num_in_queue,p->front,p->rear,p->len);
	}
	MYQUEUE_API_UNLOCK(p);
	return rt;
}

bool myQueuePop(myQueueHandle_t queue,size_t num)
{
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	if((NULL==p) || (0==num)) {
		debug_i("pop queue=%p,num=%d",p,num);
		return false;
	}
	bool rt = true;
	MYQUEUE_API_LOCK(p);
	size_t num_in_queue = NUM_IN_QUEUE(p);
	if(num <= num_in_queue) {
		p->front = (p->front + num) % (p->len);
	}
	else {
		rt = false;
		debug_i("pop failed, num=%d,inQueue=%d,front=%d,rear=%d,len=%d",\
				num,num_in_queue,p->front,p->rear,p->len);
	}
	MYQUEUE_API_UNLOCK(p);
	return rt;
}

bool myQueuePopAll(myQueueHandle_t queue)
{
	MYQUEUE_T *p = (MYQUEUE_T *)queue;
	if(NULL == p) {
		return false;
	}
	else {
		MYQUEUE_API_LOCK(p);
		p->front = p->rear;
		MYQUEUE_API_UNLOCK(p);
		return true;
	}
}

