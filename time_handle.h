/**
 * @file      : time_handle.h
 * @brief     : Linux平台时间处理相关函数头文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-01-18 15:40:44
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-01-18 huenrong        创建文件
 *
 */

#ifndef __TIME_HANDLE_H
#define __TIME_HANDLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

// 日期时间结构体
typedef struct
{
    uint16_t tm_year; // 4位年份值, 例: 1995
    uint8_t tm_mon;   // 取值范围: [1, 12]
    uint8_t tm_day;   // 取值范围: [1, 31]
    uint8_t tm_hour;  // 取值范围: [0, 23]
    uint8_t tm_min;   // 取值范围: [0, 59]
    uint8_t tm_sec;   // 取值范围: [0, 59]
} date_time_t;

/**
 * @brief  获取编译时间戳
 * @param  get_time: 输出参数: 获取到的编译时间戳(格式: 2021-05-12 15:53:41)
 */
void get_compile_time(char *get_time);

/**
 * @brief  获取系统当前时区
 * @return 时区
 */
int get_local_time_zone(void);

/**
 * @brief  获取系统当前时间字符串(精确到毫秒)
 * @param  time_buf: 输出参数, 获取到的时间串
 * @param  gap_flag: 输入参数, 获取到的时间是否需要间隔
 *                            0: 无间隔: 20180806163000616678
 *                            1: 有间隔: 2018-08-06 16:30:00.616678
 *                            2: 有间隔: 2018-08-06_16-30-00-616678
 * @return true : 成功
 * @return false: 失败
 */
bool get_time_str(char *time_buf, const uint8_t gap_flag);

/**
 * @brief  获取系统当前时间
 * @param  local_time: 输出参数, 获取到的本地时间
 * @return true : 成功
 * @return false: 失败
 */
bool get_local_time(date_time_t *local_time);

/**
 * @brief  设置系统当前时间
 * @param  local_time: 输入参数, 当前时间
 * @return true : 成功
 * @return false: 失败
 */
bool set_local_time(const date_time_t local_time);

/**
 * @brief  获取当前毫秒时间
 * @param  msec: 输出参数, 获取到的毫秒时间
 * @return true : 成功
 * @return false: 失败
 */
bool get_current_msec(uint64_t *msec);

/**
 * @brief  unix时间戳转换为本地时间
 * @param  local_time: 输出参数, 转换后的本地时间
 * @param  unix_time : 输入参数, unix时间戳
 */
void unix_time_to_local_time(date_time_t *local_time, const uint64_t unix_time);

/**
 * @brief  本地时间转换为unix时间戳
 * @param  unix_time : 输出参数, 转换后的unix时间戳
 * @param  local_time: 输入参数, 本地时间
 */
void local_time_to_unix_time(uint64_t *unix_time, const date_time_t local_time);

/**
 * @brief  从系统时间设置硬件时钟
 * @param  device: 输入参数, 指定设备(形如: /dev/rtc0)
 */
void set_hardware_clock_from_system_time(const char *device);

/**
 * @brief  从硬件时钟设置系统时间
 * @param  device: 输入参数, 指定设备(形如: /dev/rtc0)
 */
void set_system_time_from_hardware_clock(const char *device);

#ifdef __cplusplus
}
#endif

#endif // __TIME_HANDLE_H
