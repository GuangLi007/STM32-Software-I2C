# Usage Examples / 使用示例

## 1. Initialization / 初始化

```c
#include "MY_I2C.h"
#include "MY_DELAY.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    my_delay_us_init(72);     // Init delay with 72MHz SYSCLK / 初始化延时（72MHz）
    IIC_Init();                // Init I2C pins / 初始化 I2C 引脚

    while (1)
    {
        // Application / 用户代码
    }
}
```

---

## 2. Write & Read Single Byte / 单字节读写

Write `0x55` to register `0x00` on device `0xA0`, then read it back.  
向地址 `0xA0` 的器件写入 `0x55` 到寄存器 `0x00`，再读回验证。

```c
// Write / 写入
if (IIC_Write_OneByte(0xA0, 0x00, 0x55) == 0)
{
    // Write success / 写入成功
}

// Read / 读取
uint8_t val = IIC_Read_OneByte(0xA0, 0x00);
// val == 0x55
```

---

## 3. Multi-Byte Read / 多字节读取

Read 16 bytes from device `0xD0` starting at register `0x10`.  
从器件 `0xD0` 的寄存器 `0x10` 开始连续读取 16 字节。

```c
uint8_t buf[16];

if (IIC_Read_Len(0xD0, 0x10, 16, buf) == 0)
{
    // Data in buf[0..15]
}
```

---

## 4. Multi-Byte Write / 多字节写入

Write 8 bytes to device `0x90`.  
向器件 `0x90` 连续写入 8 字节。

```c
uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

if (IIC_Write_Len(0x90, 0x00, 8, data) == 0)
{
    // Write success / 写入成功
}
```

---

## 5. Delay Usage / 延时使用

```c
my_delay_us(10);     // 10 us delay / 10 微秒延时
my_delay_ms(100);    // 100 ms delay / 100 毫秒延时
```

---

## 6. Common I2C Device Examples / 常见器件示例

### OLED SSD1306 (addr 0x78)

```c
IIC_Write_OneByte(0x78, 0x00, 0xAE);  // Display OFF / 关显示
IIC_Write_OneByte(0x78, 0x00, 0xAF);  // Display ON / 开显示
```

### Temperature Sensor LM75 (addr 0x9A)

```c
uint8_t temp = IIC_Read_OneByte(0x9A, 0x00);
// temp = temperature value / 温度值
```

### EEPROM AT24Cxx (addr 0xA0)

```c
// Write byte / 写入字节
IIC_Write_OneByte(0xA0, 0x10, 0xAA);

// Read byte / 读取字节
uint8_t val = IIC_Read_OneByte(0xA0, 0x10);
```

---

## 7. Note / 注意

- Device address is **7-bit left-shifted by 1** (as used in datasheets).  
  器件地址为**左移 1 位的 7 位地址**（与数据手册一致）。
- Example: LM75 address `0x9A` = `0x4D << 1`.
- Modify pins in `MY_I2C.h` to match your hardware.  
  根据硬件修改 `MY_I2C.h` 中的引脚定义。
