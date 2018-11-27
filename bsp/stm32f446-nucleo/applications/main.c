/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>

#include <drivers/pin.h>

#define LED1 46 //PF9--21，在 drv_gpio.c 文件 pin_index pins[] 中查到 PF9 编号为 21
#define LED3 75 //PF10--22，在 drv_gpio.c 文件 pin_index pins[] 中查到 PF10 编号为 22
#define LED2 137 //PF10--22，在 drv_gpio.c 文件 pin_index pins[] 中查到 PF10 编号为 22
 void led_thread_entry(void* parameter)
{
    // 设置管脚为输出模式
    rt_pin_mode(LED1, PIN_MODE_OUTPUT);
    // 设置管脚为输出模式
    rt_pin_mode(LED2, PIN_MODE_OUTPUT);
    // 设置管脚为输出模式
    rt_pin_mode(LED3, PIN_MODE_OUTPUT);

    while (1)
    {
        // 输出低电平，LED0 亮
        rt_pin_write(LED1, PIN_LOW);
        // 输出低电平，LED1 亮
        rt_pin_write(LED2, PIN_LOW);
        // 输出低电平，LED3 亮
        rt_pin_write(LED3, PIN_LOW);
        // 挂起 500ms
        rt_thread_delay(rt_tick_from_millisecond(85));

        // 输出高电平，LED0 灭
        rt_pin_write(LED1, PIN_HIGH);
        // 输出高电平，LED1 灭
        rt_pin_write(LED2, PIN_HIGH);
        // 输出低电平，LED3 亮
        rt_pin_write(LED3, PIN_HIGH);        
        // 挂起 500ms
        rt_thread_delay(rt_tick_from_millisecond(50));
    }
}

int main(void)
{
    /* user app entry */
    rt_thread_t tid;// 线程句柄
    /* 创建 led 线程 */
    tid = rt_thread_create("led",
                    led_thread_entry,
                    RT_NULL,
                    1024,
                    3,
                    10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}

