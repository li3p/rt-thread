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
#include <packages/button-v1.0.0/button.h>

#define LED1 46 //PF9--21，在 drv_gpio.c 文件 pin_index pins[] 中查到 PF9 编号为 21
#define LED3 75 //PF10--22，在 drv_gpio.c 文件 pin_index pins[] 中查到 PF10 编号为 22
#define LED2 137 //PF10--22，在 drv_gpio.c 文件 pin_index pins[] 中查到 PF10 编号为 22


Button_t button1;



#define SLICE 25
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
        rt_thread_delay(rt_tick_from_millisecond(SLICE));

        // 输出高电平，LED0 灭
        rt_pin_write(LED1, PIN_HIGH);
        // 输出高电平，LED1 灭
        rt_pin_write(LED2, PIN_HIGH);
        // 输出低电平，LED3 亮
        rt_pin_write(LED3, PIN_HIGH);        
        // 挂起 500ms
        rt_thread_delay(rt_tick_from_millisecond(SLICE));
    }
}

#define KEY0    7   //PC13--2，在 drv_gpio.c 文件 pin_index pins[] 中查到 PE3 编号为 2
#define KEY1    7   //PC13--2，在 drv_gpio.c 文件 pin_index pins[] 中查到 PE3 编号为 2

uint8_t read_button_level(void)
{
  return rt_pin_read(KEY1);
}

void Btn2_Dowm_CallBack(void* args){

    rt_kprintf("button driver, key0 down! \n");
}

void Btn2_Double_CallBack(void* args){
    rt_kprintf("button driver, key0 double! \n");
}


void Btn2_Long_CallBack(void* args){
    rt_kprintf("button driver, key0 long! \n");
}

void Btn2_Cont_CallBack(void* args){
    rt_kprintf("button driver, key0 contiue! \n");
}


void key_thread_entry(void* parameter)
{
    //PE2、PE3 设置上拉输入
       rt_pin_mode(KEY1, PIN_MODE_INPUT);

    Button_Create("button1", 
        &button1,
        read_button_level,
        PIN_HIGH
    );

    Button_Attach(&button1,BUTTON_DOWM,Btn2_Dowm_CallBack);		//按键单击
    Button_Attach(&button1,BUTTON_DOUBLE,Btn2_Double_CallBack);	//双击
    Button_Attach(&button1,BUTTON_LONG,Btn2_Long_CallBack);		//长按
    Button_Attach(&button1,BUTTON_CONTINUOS,Btn2_Cont_CallBack);		//长按
				
    while (1)
    {
        // // 检测到低电平，即按键 1 按下了
        // if (rt_pin_read(KEY1) == PIN_HIGH)
        // {
        //     rt_kprintf("key1 pressed!\n");
        // }

        Button_Process();

        // 挂起 10ms
        rt_thread_delay(rt_tick_from_millisecond(25));
    }
}

void hdr_callback(void *args)// 回调函数
{
    char *a = args;// 获取参数
    rt_kprintf("key0 down! %s\n",a);
}

void irq_thread_entry(void* parameter)
{
    // 上拉输入
    rt_pin_mode(KEY0, PIN_MODE_INPUT_PULLUP);
    // 绑定中断，下降沿模式，回调函数名为 hdr_callback
    rt_pin_attach_irq(KEY0, PIN_IRQ_MODE_RISING, hdr_callback, (void*)"callback args");
    // 使能中断
    rt_pin_irq_enable(KEY0, PIN_IRQ_ENABLE);

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

          /* 创建 key 线程 */
    tid = rt_thread_create("key2",
                    key_thread_entry,
                    RT_NULL,
                    1024,
                    2,
                    10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

//   /* 创建 key 线程 */
//     tid = rt_thread_create("key2",
//                     irq_thread_entry,
//                     RT_NULL,
//                     1024,
//                     2,
//                     10);
//     /* 创建成功则启动线程 */
//     if (tid != RT_NULL)
//         rt_thread_startup(tid);

    return 0;
}

