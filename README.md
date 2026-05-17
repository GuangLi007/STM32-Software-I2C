# STM32 Software I2C

Software I2C (bit-bang) driver for STM32 HAL library, with SysTick-based microsecond delay.

STM32 软件 I2C 驱动，基于 HAL 库，采用 IO 模拟时序，附带 SysTick 微秒延时。

---

## Features / 功能

- I2C master mode / 主机模式
- Standard bit-bang时序 / 标准 IO 模拟时序
- 7-bit device addressing / 7 位器件地址
- Single/multi-byte read & write / 单字节和多字节读写
- Configurable GPIO pins / 引脚可配置
- SysTick-based us delay / SysTick 微秒延时

---

## Pin Configuration / 引脚配置

| Signal | Pin   | Port |
|--------|-------|------|
| SDA    | PH13  | GPIOH |
| SCL    | PH15  | GPIOH |

Modify in `MY_I2C.h` to change pins / 可在头文件中修改引脚:

```c
#define IIC_SDA_PIN      GPIO_PIN_13
#define IIC_SCL_PIN      GPIO_PIN_15
#define IIC_GPIO_PORT    GPIOH
```

---

## API / 接口

### I2C

```c
void     IIC_Init(void);
void     IIC_Start(void);
void     IIC_Stop(void);
void     IIC_Send_Byte(uint8_t txd);
uint8_t  IIC_Read_Byte(uint8_t ack);
uint8_t  IIC_Wait_Ack(void);
void     IIC_Ack(void);
void     IIC_NAck(void);

uint8_t  IIC_Read_OneByte(uint8_t DevAddr, uint8_t RegAddr);
uint8_t  IIC_Write_OneByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t data);
uint8_t  IIC_Read_Len(uint8_t DevAddr, uint8_t RegAddr, uint16_t len, uint8_t* buf);
uint8_t  IIC_Write_Len(uint8_t DevAddr, uint8_t RegAddr, uint16_t len, uint8_t* buf);
```

### Delay

```c
void my_delay_us(uint32_t n);
void my_delay_ms(uint16_t n);
void my_delay_us_init(uint8_t SYSCLK);
```

---

## Usage Example / 使用示例

```c
#include "MY_I2C.h"

// Read/write an I2C EEPROM at address 0xA0
IIC_Write_OneByte(0xA0, 0x00, 0x55);        // write 0x55 to addr 0x00
uint8_t val = IIC_Read_OneByte(0xA0, 0x00);  // read back
```

---

## Files / 文件

| File          | Description          | 说明         |
|---------------|----------------------|-------------|
| `MY_I2C.c`    | I2C driver           | I2C 驱动     |
| `MY_I2C.h`    | I2C header           | I2C 头文件   |
| `MY_DELAY.c`  | SysTick delay        | 延时函数     |
| `MY_DELAY.h`  | Delay header         | 延时头文件   |
