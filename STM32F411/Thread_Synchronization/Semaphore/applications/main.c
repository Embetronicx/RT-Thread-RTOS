#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <drv_common.h>
#include <rtdevice.h>

#define INPUT_PIN   GET_PIN(A, 0)       //PA0
#define LED_PIN     GET_PIN(C, 13)      //PC13

static rt_thread_t threadId1 = RT_NULL;
static rt_thread_t threadId2 = RT_NULL;

static rt_sem_t semaphore_sync = RT_NULL;

/* Thread 1 */
static void thread1_function(void *parameter)
{
    rt_pin_mode( INPUT_PIN , PIN_MODE_INPUT_PULLUP);

    while (1)
    {
        if( rt_pin_read(INPUT_PIN) == 0 )
        {
            LOG_D("Button Pressed - Released the Semaphore!!!\n");

            /* Release the semaphore */
            rt_sem_release(semaphore_sync);
        }
        rt_thread_mdelay(200);
    }
}

/* Thread 2 */
static void thread2_function(void *parameter)
{
    rt_pin_mode( LED_PIN , PIN_MODE_OUTPUT);
    rt_pin_write(LED_PIN, PIN_HIGH);

    while (1)
    {
        rt_sem_take(semaphore_sync, RT_WAITING_FOREVER);
        LOG_D("Got Semaphore - Blink the LED!!!\n");

        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(2000);
        rt_pin_write(LED_PIN, PIN_HIGH);
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
                            5,                  //Thread priority
                            10);                //Time slice in ticks

    /* Create thread 2 */
    threadId2 = rt_thread_create("thread2_fn",  //Name
                            thread2_function,   //Function address
                            RT_NULL,            //Thread function parameter
                            1024,               //Stack Size
                            5,                  //Thread priority
                            10);                //Time slice in ticks

    /* Create the Semaphore */
    semaphore_sync = rt_sem_create( "Semaphore_Sync",            //Name
                                    0,                           //Initial value
                                    RT_IPC_FLAG_FIFO );          //Flag

    if( semaphore_sync == RT_NULL )
    {
        LOG_D("Semaphore Creation Failed!!!\n");
        return RT_EOK;
    }

    /* Start both threads */
    rt_thread_startup(threadId1);
    rt_thread_startup(threadId2);

    while(1);

    return RT_EOK;
}
