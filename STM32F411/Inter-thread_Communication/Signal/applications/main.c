#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <drv_common.h>
#include <rtdevice.h>

static rt_thread_t threadId1 = RT_NULL;
static rt_thread_t threadId2 = RT_NULL;

/* Signal process function for thread 2 signal handler */
void thread2_signal_handler1(int sig)
{
    rt_kprintf("Thread 2 Handler 1: Received signal %d\n", sig);
}

/* Signal process function for thread 2 signal handler */
void thread2_signal_handler2(int sig)
{
    rt_kprintf("Thread 2 Handler 2: Received signal %d\n", sig);
}


/* Thread 1 */
static void thread1_function(void *parameter)
{
    uint32_t count = 0;
    while (1)
    {
        count++;
        LOG_D("Thread 1: count = %d\n", count);

        if( count & 0x01 )
        {
            /* Send signal SIGUSR1 to thread 2 */
            rt_thread_kill(threadId2, SIGUSR1);
        }
        else
        {
            /* Send signal SIGUSR2 to thread 2 */
            rt_thread_kill(threadId2, SIGUSR2);
        }

        rt_thread_mdelay(1000);
    }
}

/* Thread 2 */
static void thread2_function(void *parameter)
{
    /* Install signal */
    rt_signal_install(SIGUSR1, thread2_signal_handler1);
    rt_signal_install(SIGUSR2, thread2_signal_handler2);

    /* Unblock the signal */
    rt_signal_unmask(SIGUSR1);
    rt_signal_unmask(SIGUSR2);

    while (1)
    {
        /* Do nothing or something */
    }
}

int main(void)
{
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
