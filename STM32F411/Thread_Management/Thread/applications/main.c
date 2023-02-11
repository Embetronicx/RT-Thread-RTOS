#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <drv_common.h>
#include <rtdevice.h>

#define LED1_PIN    GET_PIN(A, 2)       //PA2
#define LED2_PIN    GET_PIN(C, 13)      //PC13

static rt_thread_t threadId1 = RT_NULL;
static rt_thread_t threadId2 = RT_NULL;

/* Thread 1 */
static void thread1_function(void *parameter)
{
    long value = 0;
    rt_pin_mode( LED1_PIN , PIN_MODE_OUTPUT);

    while (1)
    {
        LOG_D("Thread 1 is Running!\n");
        rt_pin_write(LED1_PIN, value);
        value = ~value;
        rt_thread_mdelay(1000);
    }
}

/* Thread 2 */
static void thread2_function(void *parameter)
{
    long value = 0;
    rt_pin_mode( LED2_PIN , PIN_MODE_OUTPUT);

    while (1)
    {
        LOG_D("Thread 2 is Running!\n");
        rt_pin_write(LED2_PIN, value);
        value = ~value;
        rt_thread_mdelay(2000);
    }
}


int main(void)
{
    /* Create thread 1 */
    threadId1 = rt_thread_create("thread1_fn",  //Name
                            thread1_function,   //Function address
                            RT_NULL,            //Thread function parameter
                            1024,               //Stack Size
                            5,                  //Thread priority
                            10);                //Time slice in ticks

    /* Create thread 1 */
    threadId2 = rt_thread_create("thread2_fn",  //Name
                            thread2_function,   //Function address
                            RT_NULL,            //Thread function parameter
                            1024,               //Stack Size
                            5,                  //Thread priority
                            10);                //Time slice in ticks

    /* Start both threads */
    rt_thread_startup(threadId1);
    rt_thread_startup(threadId2);

    while(1);

    return RT_EOK;
}
