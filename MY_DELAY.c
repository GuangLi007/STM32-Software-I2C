/******************************************
文件：MY_DELAY.c  
功能：延时相关接口  
只能運行在STM32的單片機上
参考：1. https://zhuanlan.zhihu.com/p/349024420  
******************************************/

#include "MY_DELAY.h"

/******************************************
名称：my_delay_ms  
功能：毫秒级延时  
输入参数：n-延时时间（ms）  
返回值：无  
说明：直接調用HAL庫的延時函數 
******************************************/
void my_delay_ms(uint16_t n)
{
    HAL_Delay(n);  // 使用HAL库的毫秒延时
}


/******************************************
名称：my_delay_us  
功能：微秒级延时  
输入参数：n-延时时间（us）  
返回值：无  
说明：基于SysTick的高精度延時 
不涉及中斷優先級，可以不修改SISTICK的中斷Priority
******************************************/
void my_delay_us(uint32_t n)
{

    uint32_t startval, tickn, delays, wait, sysc;

    // 获取当前SysTick值和系统时钟计数
    startval = SysTick->VAL;               // 当前SysTick计数器值
    tickn = HAL_GetTick();                 // 当前毫秒计时值
    sysc = HAL_RCC_GetSysClockFreq() / 1000; // 计算每毫秒的时钟周期数（如72MHz时为72000）

    // 计算需要延时的时钟周期数
    delays = n * sysc / 1000;              // 将us转换为时钟周期数

    // 分情况处理延时
    if (delays > startval) 
    {
        // 延时超过当前SysTick剩余值，需等待下一次重载
        while (HAL_GetTick() == tickn);    // 等待tick增加
        wait = sysc + startval - delays;   // 计算剩余等待周期
        while (wait < SysTick->VAL);       // 等待SysTick递减到目标值
    } 
    else 
    {
        // 直接在当前周期内完成延时
        wait = startval - delays;
        while (wait < SysTick->VAL && HAL_GetTick() == tickn); // 等待且不跨tick
    }
}