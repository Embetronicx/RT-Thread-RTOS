#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <drv_common.h>
#include <rtdevice.h>

#define FIRST_EVENT  (1 << 0)
#define SECOND_EVENT (1 << 1)

static struct rt_event event;   //static event

static rt_thread_t threadId1 = RT_NULL;
static rt_thread_t threadId2 = RT_NULL;

/* Thread 1 */
static void thread1_function(void *parameter)
{
    uint8_t count = 0;
    while (1)
    {
        count++;
        LOG_D("Thread 1: count = %d\n", count);
        if( count == 5 )
        {
            //send 1st event
            rt_event_send(&event, FIRST_EVENT);
        }
        else if( count == 10 )
        {
            //send 2nd event and clear the count
            rt_event_send(&event, SECOND_EVENT);
            count = 0;
        }
        rt_thread_mdelay(1000);
    }
}

/* Thread 2 */
static void thread2_function(void *parameter)
{
    uint32_t received_evt;
    while (1)
    {
        if (rt_event_recv(  &event,
                            (FIRST_EVENT | SECOND_EVENT),
                            RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER,
                            &received_evt) == RT_EOK)
        {
            LOG_D("Thread 2: Received both Events\n");
        }
    }
}

int main(void)
{
    /* Initialize event object */
    if( rt_event_init(&event, "event", RT_IPC_FLAG_FIFO) != RT_EOK )
    {
        rt_kprintf("Event init failed.\n");
        return -1;
    }

    /* Create thread 1 */
    threadId1 = rt_thread_create("thread1_fn",  //Name
                            thread1_function,   //Function address
                            RT_NULL,            //Thread function parameter
                            1024,               //Stack Size
                            1,                  //Thread priority
                            2000);              //Time slice in ticks

    /* Create thread 2 */
    threadId2 = rt_thread_create("thread2_fn",  //Name
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
