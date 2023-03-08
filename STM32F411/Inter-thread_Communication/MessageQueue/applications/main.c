#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <drv_common.h>
#include <rtdevice.h>

/* Message queue control block */
static struct rt_messagequeue *mq;

static rt_thread_t threadId1 = RT_NULL;
static rt_thread_t threadId2 = RT_NULL;

/* Thread 1 */
static void thread1_function(void *parameter)
{
    uint32_t count = 0;
    while (1)
    {
        count++;
        LOG_D("Thread 1: count = %d\n", count);
        rt_mq_send(mq, (void*)&count, sizeof(uint32_t));
        rt_thread_mdelay(1000);
    }
}

/* Thread 2 */
static void thread2_function(void *parameter)
{
    uint32_t received_message = 0;
    while (1)
    {
        /* Receive message from the queue */
        if (rt_mq_recv(mq, &received_message, sizeof(uint32_t), RT_WAITING_FOREVER) == RT_EOK)
        {
            LOG_D("Thread2: Got a message, the message:%u\n", received_message);
        }
    }
}

int main(void)
{
    /* create queue object */
    mq = rt_mq_create("etx_mq", sizeof(uint32_t), 10, RT_IPC_FLAG_FIFO);
    if( mq  == RT_NULL )
    {
        LOG_D("Queue init failed.\n");
        return -1;
    }

    /* Create thread 1 */
    threadId1 = rt_thread_create("thread1_fn",        //Name
                                  thread1_function,   //Function address
                                  RT_NULL,            //Thread function parameter
                                  1024,               //Stack Size
                                  1,                  //Thread priority
                                  2000);              //Time slice in ticks

    /* Create thread 2 */
    threadId2 = rt_thread_create("thread2_fn",        //Name
                                  thread2_function,   //Function address
                                  RT_NULL,            //Thread function parameter
                                  1024,               //Stack Size
                                  1,                  //Thread priority
                                  2000);              //Time slice in ticks

    /* Start both threads */
    rt_thread_startup(threadId1);
    rt_thread_startup(threadId2);

    while(1);

    return RT_EOK;
}
