#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <drv_common.h>
#include <rtdevice.h>

uint32_t global_var = 10;

static rt_thread_t threadId1 = RT_NULL;
static rt_thread_t threadId2 = RT_NULL;

/* Thread 1 */
static void thread1_function(void *parameter)
{
    while (1)
    {
        //rt_hw_interrupt_disable();
        rt_enter_critical();

        global_var++;                       //Accessing common resource
        LOG_D("Thread 1: global_var = %d\n", global_var);

        //rt_hw_interrupt_enable(0);
        rt_exit_critical();

        rt_thread_mdelay(1000);
    }
}

/* Thread 2 */
static void thread2_function(void *parameter)
{
    while (1)
    {
        //rt_hw_interrupt_disable();
        rt_enter_critical();

        global_var--;                       //Accessing common resource
        LOG_D("Thread 2: global_var = %d\n", global_var);

        //rt_hw_interrupt_enable(0);
        rt_exit_critical();


        rt_thread_mdelay(1000);
    }
}


int main(void)
{
    /* Create thread 1 */
    threadId1 = rt_thread_create("thread1_fn",  //Name
                            thread1_function,   //Function address
                            RT_NULL,            //Thread function parameter
                            1024,               //Stack Size
                            1,                  //Thread priority
                            2000);                 //Time slice in ticks

    /* Create thread 2 */
    threadId2 = rt_thread_create("thread2_fn",  //Name
                            thread2_function,   //Function address
                            RT_NULL,            //Thread function parameter
                            1024,               //Stack Size
                            1,                  //Thread priority
                            2000);                 //Time slice in ticks

    /* Start both threads */
    rt_thread_startup(threadId1);
    rt_thread_startup(threadId2);

    while(1);

    return RT_EOK;
}
