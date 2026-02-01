# MC3-S3 主板文档

## 编译命令

### 一键编译

```bash
python scripts/release.py mc3-s3
```

### 手动配置编译

```bash
idf.py set-target esp32s3
```

**配置**

```bash
idf.py menuconfig
```

选择板子

```
Xiaozhi Assistant -> Board Type -> MC3-S3
```

## 编译烧入

```bash
idf.py -DBOARD_NAME=mc3-s3 build flash
```

## 主板特性

- 基于 ESP32-S3 芯片
- 支持 240x320 分辨率 LCD 显示屏
- 支持 I2S 音频输入输出
- 支持 WiFi 连接

## 引脚定义

### 音频相关引脚
```
麦克风输入:
- WS: GPIO40
- SCK: GPIO39
- DIN: GPIO41

扬声器输出:
- DOUT: GPIO6
- BCLK: GPIO5
- LRCK: GPIO4
```

### 显示相关引脚
```
- 背光: GPIO7
- MOSI: GPIO8
- MISO: GPIO13
- CLK: GPIO16
- DC: GPIO17
- RST: GPIO15
- CS: GPIO18
- 触摸CS: 未定义
```

### 其他引脚
```
- 内置LED: 未定义
- 启动按钮: 未定义
- 触摸按钮: 未定义
- 音量加按钮: 未定义
- 音量减按钮: 未定义
- 灯控: 未定义
```

## 注意事项

- 确保在编译前正确设置目标芯片为 ESP32-S3
- 首次使用前请检查所有硬件连接
- 如需修改引脚配置，请编辑 `config.h` 文件
- 如需修改显示配置，请在 `menuconfig` 中选择相应的 LCD 类型
