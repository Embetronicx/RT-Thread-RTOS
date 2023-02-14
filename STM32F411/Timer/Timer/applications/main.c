/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-14     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* Timer Control Block */
static rt_timer_t timer1;           //Dynamic Periodic timer
static rt_timer_t timer2;           //Dynamic One shot timer
static struct rt_timer timer3;      //Static Periodic timer
static rt_timer_t timer4;           //Dynamic Periodic soft timer

int32_t count = 0;

/* Timer 1 Function */
static void timer_1_callback(void *parameter)
{
    LOG_D("Timeout: Timer 1 Function!!!");
}

/* Timer 2 Function */
static void timer_2_callback(void *parameter)
{
    LOG_D("Timeout: Timer 2 Function!!!");
}

/* Timer 3 Function */
static void timer_3_callback(void *parameter)
{
    LOG_D("Timeout: Timer 3 Function!!!");

    if( count++ >= 2 )
    {
        rt_tick_t timeNow;

        //get time
        rt_timer_control( &timer3, RT_TIMER_CTRL_GET_TIME, &timeNow );

        //Stop timer
        rt_timer_stop(&timer3);

        LOG_D("Timer 3 Time is %dms, Stopping Timer 3", timeNow);
    }
}

/* Timer 4 Function */
static void timer_4_callback(void *parameter)
{
    LOG_D("Timeout: Timer 4 Function!!!");
}

int main(void)
{
    /* Create Timer 1 Periodic Timer */
    timer1 = rt_timer_create( "Timer1",
                              timer_1_callback,
                              RT_NULL,
                              1000,
                              RT_TIMER_FLAG_PERIODIC );

    /* Start Timer 1*/
    if (timer1 != RT_NULL)
    {
        rt_timer_start(timer1);
    }

    /* Create Timer 2 One Shot Timer */
    timer2 = rt_timer_create( "Timer2",
                              timer_2_callback,
                              RT_NULL,
                              2000,
                              RT_TIMER_FLAG_ONE_SHOT );

    /* Start Timer 2 */
    if (timer2 != RT_NULL)
    {
        rt_timer_start(timer2);
    }

    /* Create Timer 3 Static Periodic Timer */
     rt_timer_init(  &timer3,
                     "Timer3",
                     timer_3_callback,
                     RT_NULL,
                     3000,
                     RT_TIMER_FLAG_PERIODIC );

    /* Start Timer 3*/
    rt_timer_start(&timer3);

    /* Create Timer 4 Periodic Soft Timer */
    timer4 = rt_timer_create( "Timer4",
                              timer_4_callback,
                              RT_NULL,
                              4000,
                              RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER );

    /* Start Timer 4 */
    if (timer4 != RT_NULL)
    {
        rt_timer_start(timer4);
    }

    return RT_EOK;
}
