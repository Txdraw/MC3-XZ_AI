# Alarm

好用的话给个⭐️吧!!!! 粉丝群719932592, 使用代码需要标清楚来源

## 使用

1.   正常配置使用小智的各个属性
2.   选择使用闹钟
3.   板子有SD卡的时候可以开启使用SD卡关机保持的功能, 需要填写SD卡的挂载点(根据板子的初始化SD卡时候的设置)

![image-20251121102219589](https://picture-01-1316374204.cos.ap-beijing.myqcloud.com/mac-picture/image-20251121102219589.png)

```c
#define MOUNT_POINT "/sdcard"
```

![image-20251121151336115](https://picture-01-1316374204.cos.ap-beijing.myqcloud.com/mac-picture/image-20251121151336115.png)

## 移植

1.   添加文件

![image-20251121160539063](https://picture-01-1316374204.cos.ap-beijing.myqcloud.com/mac-picture/image-20251121160539063.png)

![image-20251121160647312](https://picture-01-1316374204.cos.ap-beijing.myqcloud.com/mac-picture/image-20251121160647312.png)

2.   添加配置选项

![image-20251121160735374](https://picture-01-1316374204.cos.ap-beijing.myqcloud.com/mac-picture/image-20251121160735374.png)

3.   根据宏定义CONFIG_USE_ALARM进行对应位置代码的添加
4.   添加对应的语言文件里面的ALARM字段, 例: `main/assets/locales/zh-CN/language.json`(我这里其他语言是使用AI翻译添加呆的)
5.   使用`scripts/ogg_converter/xiaozhi_ogg_converter.py`把音频文件转为铃声, 重命名为`alarm_ring.ogg`, 放在assets/common文件下面

![image-20251121100659012](https://picture-01-1316374204.cos.ap-beijing.myqcloud.com/mac-picture/image-20251121100659012.png)

+   可选

4.   SD卡初始化, 在板级驱动里面添加SD卡驱动, 下面是一个示例

```c
    void SDCardInit(){
        esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = true,   // 如果挂载不成功是否需要格式化SD卡
            .max_files = 5, // 允许打开的最大文件数
            .allocation_unit_size = 16 * 1024  // 分配单元大小
        };
        
        sdmmc_card_t *card;
        const char mount_point[] = MOUNT_POINT;
        ESP_LOGI(TAG, "Initializing SD card");
        ESP_LOGI(TAG, "Using SDMMC peripheral");
    
        sdmmc_host_t host = SDMMC_HOST_DEFAULT(); // SDMMC主机接口配置
        sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT(); // SDMMC插槽配置
        slot_config.width = 1;  // 设置为1线SD模式
        slot_config.clk = GPIO_NUM_47; 
        slot_config.cmd = GPIO_NUM_48;
        slot_config.d0 = GPIO_NUM_21;
        slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP; // 打开内部上拉电阻
    
        ESP_LOGI(TAG, "Mounting filesystem");
        esp_err_t ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card); // 挂载SD卡
    
        if (ret != ESP_OK) {  // 如果没有挂载成功
            if (ret == ESP_FAIL) { // 如果挂载失败
                ESP_LOGE(TAG, "Failed to mount filesystem. ");
            } else { // 如果是其它错误 打印错误名称
                ESP_LOGE(TAG, "Failed to initialize the card (%s). ", esp_err_to_name(ret));
            }
            return;
        }
        ESP_LOGI(TAG, "Filesystem mounted"); // 提示挂载成功
        sdmmc_card_print_info(stdout, card); // 终端打印SD卡的一些信息
    }
```

