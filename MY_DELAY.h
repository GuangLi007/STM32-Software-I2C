/******************************************
文件：MY_DELAY.h  
功能：延时相关接口  
创建时间：2021-07-26 作者  
最后修改：2021-07-26 作者  
******************************************/

#ifndef __MY_DELAY_H
#define __MY_DELAY_H

#include "main.h"

#ifdef __cplusplus			//C++���
extern "C" {
#endif

void my_delay_ms(uint16_t n);
void my_delay_us_init(uint8_t SYSCLK);
void my_delay_us(uint32_t n);

#ifdef __cplusplus			//C++���
}
#endif

#endif