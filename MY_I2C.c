/******************************************
*******************************************
文件：MY_USART.c  
功能：I2C 相關接口  
創建時間：2021-07-15 作者  
最後修改：2021-07-26 作者  
說明：SDA 和 SCL 需要配置為開漏輸出  
參考：1. 正點原子 F429 官方例程  
*******************************************
******************************************/

#include "MY_I2C.h"

/******************************************
名稱：SDA_OUT  
功能：配置 SDA 為輸出模式  
輸入參數：無  
返回值：無  
說明：用於 I2C 主機發送數據時  
******************************************/
void SDA_OUT()
{
    GPIO_InitTypeDef GPIO_Initure;
    
    GPIO_Initure.Pin = IIC_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽輸出
    GPIO_Initure.Pull = GPIO_PULLUP;          // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速模式
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_Initure);
}

/******************************************
名稱：SDA_IN  
功能：配置 SDA 為輸入模式  
輸入參數：無  
返回值：無  
說明：用於 I2C 主機接收數據時  
******************************************/
void SDA_IN()
{
    GPIO_InitTypeDef GPIO_Initure;
    
    GPIO_Initure.Pin = IIC_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;      // 輸入模式
    GPIO_Initure.Pull = GPIO_PULLUP;          // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速模式
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_Initure);
}

/******************************************
名稱：IIC_Init  
功能：初始化 I2C 接口  
輸入參數：無  
返回值：無  
說明：此部分建議使用 CubeMX 自動生成  
******************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   // 啟用 GPIOB 時鐘
    
    // 配置 PH4, PH5（SCL 和 SDA）
    GPIO_Initure.Pin = IIC_SDA_PIN | IIC_SCL_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽輸出
    GPIO_Initure.Pull = GPIO_PULLUP;          // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; // 高速模式
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_Initure);
    
    IIC_SDA_1;  // 初始狀態：SDA 拉高
    IIC_SCL_1;  // 初始狀態：SCL 拉高
}

/******************************************
名称：IIC_Start  
功能：产生 I2C 起始信号  
输入参数：无  
返回值：无  
说明：SCL高电平时，SDA从高变低  
******************************************/
void IIC_Start(void)
{
    SDA_OUT();     // SDA设为输出模式
    IIC_SDA_1;     // SDA拉高  
    IIC_SCL_1;     // SCL拉高
    my_delay_us(4);
    IIC_SDA_0;     // START条件：SCL高时，SDA从高变低
    my_delay_us(4);
    IIC_SCL_0;     // 钳住I2C总线，准备发送数据
}

/******************************************
名称：IIC_Stop  
功能：产生 I2C 停止信号  
输入参数：无  
返回值：无  
说明：SCL高电平时，SDA从低变高  
******************************************/
void IIC_Stop(void)
{
    SDA_OUT();     // SDA设为输出模式
    IIC_SCL_0;
    IIC_SDA_0;     // 准备STOP条件
    my_delay_us(4);
    IIC_SCL_1;     // SCL拉高
    my_delay_us(4);          
    IIC_SDA_1;     // STOP条件：SCL高时，SDA从低变高
}

/******************************************
名称：IIC_Wait_Ack  
功能：等待从机ACK应答  
输入参数：无  
返回值：1-应答失败，0-应答成功  
说明：超时检测（250次尝试）  
******************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();      // SDA设为输入模式（检测从机ACK）  
    IIC_SDA_1;     // 释放SDA线
    my_delay_us(1);      
    IIC_SCL_1;     // SCL拉高（从机应在此时拉低SDA）
    my_delay_us(1);      
    
    while(READ_SDA) // 检测SDA是否为低（ACK）
    {
        ucErrTime++;
        if(ucErrTime > 250) // 超时处理
        {
            IIC_Stop();     // 强制停止总线
            return 1;      // 应答失败
        }
    }
    IIC_SCL_0;    // SCL拉低，结束ACK检测
    return 0;     // 应答成功
}

/******************************************
名称：IIC_Ack  
功能：主机发送ACK应答  
输入参数：无  
返回值：无  
说明：SCL低时拉低SDA，再产生一个脉冲  
******************************************/
void IIC_Ack(void)
{
    IIC_SCL_0;    // SCL拉低
    SDA_OUT();    // SDA设为输出模式
    IIC_SDA_0;    // SDA拉低（ACK）
    my_delay_us(2);
    IIC_SCL_1;    // SCL拉高（从机检测ACK）
    my_delay_us(2);
    IIC_SCL_0;    // SCL拉低，结束ACK
}

/******************************************
名称：IIC_NAck  
功能：主机发送NACK（非应答）  
输入参数：无  
返回值：无  
说明：SCL低时拉高SDA，再产生一个脉冲  
******************************************/    
void IIC_NAck(void)
{
    IIC_SCL_0;    // SCL拉低
    SDA_OUT();    // SDA设为输出模式
    IIC_SDA_1;    // SDA拉高（NACK）
    my_delay_us(2);
    IIC_SCL_1;    // SCL拉高（从机检测NACK）
    my_delay_us(2);
    IIC_SCL_0;    // SCL拉低，结束NACK
}

/******************************************
名称：IIC_Send_Byte  
功能：发送一个字节（MSB first）  
输入参数：txd-待发送的数据  
返回值：无  
说明：每个bit在SCL上升沿被采样  
******************************************/      
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    SDA_OUT();    // SDA设为输出模式  
    IIC_SCL_0;    // SCL拉低，准备数据传输
    
    for(t = 0; t < 8; t++) // 从高位到低位依次发送
    {
        if((txd & 0x80) >> 7) // 取最高位
            IIC_SDA_1;
        else
            IIC_SDA_0;
        txd <<= 1;          // 左移，准备下一位
        
        my_delay_us(2);     // 保持数据稳定（TEA5767需此延迟）
        IIC_SCL_1;          // SCL拉高（从机采样）
        my_delay_us(2); 
        IIC_SCL_0;          // SCL拉低，继续下一位
        my_delay_us(2);
        
    } 
}

/******************************************
名稱：IIC_Read_Byte  
功能：讀取一個字節  
輸入參數：  
    ack=1 時，發送 ACK（確認）  
    ack=0 時，發送 nACK（非確認）  
返回值：讀取的數據  
說明：  
******************************************/  
uint8_t IIC_Read_Byte(unsigned char ack)  
{  
    unsigned char i, receive = 0;  
    SDA_IN();  // SDA 設置為輸入模式（準備讀取數據）  

    for (i = 0; i < 8; i++)  
    {  
        IIC_SCL_0;  // 拉低 SCL（時鐘線）  
        my_delay_us(2);  // 短暫延遲  
        IIC_SCL_1;  // 拉高 SCL（時鐘上升沿，讀取數據）  

        receive <<= 1;  // 左移 1 位，準備接收下一位  
        if (READ_SDA)    // 讀取 SDA（數據線）的值  
            receive++;   // 如果 SDA=1，則當前位設為 1  
        my_delay_us(1);  // 短暫延遲  
    }  

    if (!ack)  
        IIC_NAck();  // 發送 nACK（非確認，表示讀取結束）  
    else  
        IIC_Ack();   // 發送 ACK（確認，表示繼續讀取）  

    return receive;  // 返回讀取的數據  
}

/******************************************
名稱：IIC_Read_OneByte
功能：讀取一個字節
輸入參數：器件地址，寄存器地址	
返回值：讀取的數據
說明：
******************************************/
uint8_t IIC_Read_OneByte(uint8_t DeviceAddr, uint8_t ReadAddr)
{				  
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte((DeviceAddr<<1)|0); //發送器件地址+寫命令
    IIC_Wait_Ack();             //等待應答
    IIC_Send_Byte(ReadAddr);         //寫寄存器地址
    IIC_Wait_Ack();             //等待應答
    IIC_Start();                
    IIC_Send_Byte((DeviceAddr<<1)|1); //發送器件地址+讀命令
    IIC_Wait_Ack();             //等待應答
    res=IIC_Read_Byte(0);		//讀取數據,發送nACK  
    IIC_Stop();                 //產生一個停止條件
    return res;  
}

/******************************************
名稱：IIC_Write_OneByte
功能：寫入一個字節
輸入參數：器件地址，寄存器地址，要寫入的數據	
返回值：0，發送成功
        1，發送失敗
說明：
******************************************/
uint8_t IIC_Write_OneByte(uint8_t DeviceAddr, uint8_t WriteAddr, uint8_t data)
{				   	  	    																 
    IIC_Start();
    IIC_Send_Byte((DeviceAddr<<1)|0); //發送器件地址+寫命令
    if(IIC_Wait_Ack())          //等待應答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(WriteAddr);         //寫寄存器地址
    IIC_Wait_Ack();             //等待應答
    IIC_Send_Byte(data);        //發送數據
    if(IIC_Wait_Ack())          //等待ACK
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}


/******************************************
名称：IIC_Read_Len  
功能：连续读取多个字节  
输入参数：
    DeviceAddr - 器件地址  
    ReadAddr   - 寄存器地址  
    len        - 数据长度  
    pData      - 数据存储指针  
返回值：0-读取成功，1-读取失败  
说明：支持单字节和多字节读取  
******************************************/
uint8_t IIC_Read_Len(uint8_t DeviceAddr, uint8_t ReadAddr, uint16_t len, uint8_t* pData)
{
    // 1. 发送起始信号
    IIC_Start();
    
    // 2. 发送器件地址 + 写命令
    IIC_Send_Byte((DeviceAddr << 1) | 0); 
    if (IIC_Wait_Ack())  // 检测从机应答
    {
        IIC_Stop();
        return 1;  // 无应答，返回失败
    }
    
    // 3. 发送寄存器地址
    IIC_Send_Byte(ReadAddr);
    IIC_Wait_Ack();  // 等待应答
    
    // 4. 重新发送起始信号 + 器件地址 + 读命令
    IIC_Start();
    IIC_Send_Byte((DeviceAddr << 1) | 1);
    IIC_Wait_Ack();  // 等待应答
    
    // 5. 连续读取数据
    while (len)
    {
        if (len == 1)
            *pData = IIC_Read_Byte(0);  // 最后一个字节发送NACK
        else
            *pData = IIC_Read_Byte(1);  // 非最后一个字节发送ACK
        len--;
        pData++;
    }
    
    // 6. 发送停止信号
    IIC_Stop();
    return 0;  // 读取成功
}


/******************************************
名称：IIC_Write_Len  
功能：连续写入多个字节  
输入参数：
    DeviceAddr - 器件地址  
    WriteAddr  - 寄存器地址  
    len        - 数据长度  
    pData      - 待写入数据指针  
返回值：0-写入成功，1-写入失败  
说明：支持单字节和多字节写入  
******************************************/
uint8_t IIC_Write_Len(uint8_t DeviceAddr, uint8_t WriteAddr, uint16_t len, uint8_t* pData)
{
    uint8_t i;
    
    // 1. 发送起始信号
    IIC_Start();
    
    // 2. 发送器件地址 + 写命令
    IIC_Send_Byte((DeviceAddr << 1) | 0);
    if (IIC_Wait_Ack())  // 检测从机应答
    {
        IIC_Stop();
        return 1;  // 无应答，返回失败
    }
    
    // 3. 发送寄存器地址
    IIC_Send_Byte(WriteAddr);
    IIC_Wait_Ack();  // 等待应答
    
    // 4. 连续写入数据
    for (i = 0; i < len; i++)
    {
        IIC_Send_Byte(pData[i]);
        if (IIC_Wait_Ack())  // 检测从机应答
        {
            IIC_Stop();
            return 1;  // 写入失败
        }
    }
    
    // 5. 发送停止信号
    IIC_Stop();
    return 0;  // 写入成功
}

/*********************  //这个是延时函数，使用MyDelay中的函数即可  *************************/
//void delay_us(uint32_t n)
//{
//uint32_t startval,tickn,delays,wait, sysc;
// 
//  startval = SysTick->VAL;
//  tickn = HAL_GetTick();
//  sysc = HAL_RCC_GetSysClockFreq() / 1000;  //SystemCoreClock / (1000U / uwTickFreq);
//  delays =n * sysc/1000; //sysc / 1000 * udelay;
//  if(delays > startval)
//    {
//      while(HAL_GetTick() == tickn)
//        {
// 
//        }
//      wait = sysc + startval - delays;
//      while(wait < SysTick->VAL)
//        {
// 
//        }
//    }
//  else
//    {
//      wait = startval - delays;
//      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
//        {
// 
//        }
//    }

//}

