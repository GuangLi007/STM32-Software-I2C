/******************************************
*******************************************

使用軟件實現IIC協議，通過修改引脚與高低電平的宏
定義可以在STMHAL庫上運行

*******************************************
******************************************/

#ifndef _MY_I2C_H
#define _MY_I2C_H

#include "main.h"
#include "MY_DELAY.h"

#ifdef __cplusplus			//C++調用判斷
extern "C" {
#endif

//修改！
//這裏使用軟件IIC，將設置的SDA，SCL設置GPIO輸出
//(這裏涉及到很多函數GPIO口的初始化和修改性能，不能用在其他單片機如MSP上)
//然後在此處修改引脚

#define IIC_SDA_PIN			GPIO_PIN_13
#define IIC_SCL_PIN			GPIO_PIN_15
#define IIC_GPIO_PORT		GPIOH

#define	IIC_SDA_1 HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SDA_PIN, 1)
#define	IIC_SDA_0 HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SDA_PIN, 0)
#define	IIC_SCL_1 HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SCL_PIN, 1)
#define	IIC_SCL_0 HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SCL_PIN, 0)
#define READ_SDA	HAL_GPIO_ReadPin(IIC_GPIO_PORT, IIC_SDA_PIN)

/*---------------------------- 内部底层函数 ----------------------------*/
void IIC_Init(void);                // 初始化IIC的IO口（已注释，建议用CubeMX配置）
void IIC_Start(void);               // 产生IIC起始信号
void IIC_Stop(void);                // 产生IIC停止信号
void IIC_Send_Byte(uint8_t txd);    // IIC發送一个字节
uint8_t IIC_Read_Byte(unsigned char ack); // IIC讀取一个字节
uint8_t IIC_Wait_Ack(void);         // IIC等待ACK信号
void IIC_Ack(void);                 // IIC發送ACK信号
void IIC_NAck(void);                // IIC發送NACK信号

/*---------------------------- 外部调用接口 ----------------------------*/
uint8_t IIC_Read_Len(uint8_t DeviceAddr, uint8_t ReadAddr, uint16_t len, uint8_t* pData);      // 连续读取
uint8_t IIC_Write_Len(uint8_t DeviceAddr, uint8_t WriteAddr, uint16_t len, uint8_t* pData);    // 连续写入
uint8_t IIC_Read_OneByte(uint8_t DeviceAddr, uint8_t ReadAddr);                               // 读取单字节
uint8_t IIC_Write_OneByte(uint8_t DeviceAddr, uint8_t WriteAddr, uint8_t data);               // 写入单字节

#ifdef __cplusplus
}
#endif

#endif /* __MY_I2C_H__ */

