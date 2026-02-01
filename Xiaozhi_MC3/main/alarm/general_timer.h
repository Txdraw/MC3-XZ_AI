/*
 * @Descripttion: 
 * @Author: Xvsenfeng helloworldjiao@163.com
 * @LastEditors: Please set LastEditors
 * Copyright (c) 2025 by helloworldjiao@163.com, All Rights Reserved. 
 */
#pragma once

#include "time.h"
#include <esp_timer.h>
#include <string>
#include <vector>
#include <mutex>

// 定时器事件类型
typedef enum {
    E_PET_TIMER_MESSAGE = 0, // 消息
    E_PET_TIMER_FUNCTION, // 函数
}e_pet_timer_type_e;


// 定时器事件回调函数
typedef struct {  
    void (*callback)(void*);  
    void* arg;  
}e_pet_timer_function;

// 定时器事件
typedef struct{
    time_t trigger_time; // 触发时间
    e_pet_timer_type_e type; // 事件类型
    int repeat_time; // 重复时间
    int random_l; // 随机时间
    int random_h; // 随机时间
    union {
        e_pet_timer_function function;
        char *message;
    };
}e_pet_timer_event_t;

typedef void (*callback_f)(void*);
typedef struct{
    callback_f callback;
    // void *arg;
}callback_t;

typedef struct{
    int tm_sec; // 秒
    int tm_min; // 分
    int tm_hour; // 时
    int re_mday; // 日
    int re_mon; // 月
    int re_year; // 年
    int repeat_second;
    int function_id;
    char message[1024];
}timer_info_t;


// 定时器类
class GeneralTimer {
private:
    std::mutex mutex_; // 互斥锁
    esp_timer_handle_t electromic_prt_timer_ = nullptr; // 定时器句柄
    std::vector<e_pet_timer_event_t> e_pet_timer_events; // 处理事件列表
public:
    GeneralTimer();
    ~GeneralTimer();
    
    void OnClockTimer(); // 时钟定时器
    void TimerEventSort(); // 定时器事件排序
    
    void TimerAddTimerEventRelative(int seconds, e_pet_timer_type_e type, int function_id, void* arg, bool repeat);
    void TimerAddTimerEventAbsolute(time_t trigger_time, e_pet_timer_type_e type, int function_id, void* arg, bool repeat); // 添加定时器事件绝对时间
    void TimerAddTimerEventRepeat(time_t trigger_time, e_pet_timer_type_e type, int function_id, void* arg, int repeat_time, int random_l, int random_h); // 添加定时器事件重复时间

    void TimerEventProcess();  // 定时器事件处理

    void TimerReadCsvTimer(); // 读取csv定时器
    void TimerAddTimerEventToCsv(e_pet_timer_event_t event);
    bool DealTimerInfo(timer_info_t *timer_info); // 添加一个定时器事件
    void CalculationNextAndRepeat(timer_info_t *csv_info, long *delta_sec, long *interval_sec); // 计算下一个触发时间以及重复时间
    void TimerUpdateCsvTimer(); // 更新csv定时器
    void ClearRinging(); // 清除响铃
    bool isRinging(); // 是否在响铃
    std::string GetAlarmMessage(); // 获取闹钟消息
};


#define SECOND_ONE_SECOND 1 
#define SECOND_ONE_MINUTE 60
#define SECOND_ONE_HOUR 3600
#define SECOND_ONE_DAY 86400
#define SECOND_ONE_WEEK 604800
#define SECOND_ONE_MONTH 2592000
#define SECOND_ONE_YEAR 31536000
