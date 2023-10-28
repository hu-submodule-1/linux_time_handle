/**
 * @file      : time_handle.c
 * @brief     : Linux平台时间处理相关函数源文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-01-18 15:41:36
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-02-17 huenrong         1. 解决get_compile_time函数错误问题
 *                                          2. 解决ubuntu20.04上stime函数报错问题
 *              2023-01-18 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "./time_handle.h"

/**
 * @brief  获取编译时间戳
 * @param  get_time: 输出参数: 获取到的编译时间戳(格式: 2021-05-12 15:53:41)
 */
void get_compile_time(char *get_time)
{
    const char month[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char compile_date[20] = {0};
    char compile_time[20] = {0};
    char str_month[4] = {0};
    int year = 0;
    int mon = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;

    snprintf(compile_date, sizeof(compile_date), "%s", __DATE__);
    snprintf(compile_time, sizeof(compile_time), "%s", __TIME__);

    sscanf(compile_date, "%s %02d %04d", str_month, &day, &year);
    sscanf(compile_time, "%02d:%02d:%02d", &hour, &min, &sec);

    for (uint8_t i = 0; i < 12; ++i)
    {
        if (0 == strncmp(str_month, month[i], 3))
        {
            mon = i + 1;

            break;
        }
    }

    snprintf(get_time, 20, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
}

/**
 * @brief  获取系统当前时区
 * @return 时区
 */
int get_local_time_zone(void)
{
    int time_zone = 0;
    time_t t1 = -1;
    struct tm *tm_local = NULL;
    time_t t2 = -1;
    struct tm *tm_utc = NULL;

    time(&t1);
    t2 = t1;

    tm_local = localtime(&t1);
    t1 = mktime(tm_local);

    tm_utc = gmtime(&t2);
    t2 = mktime(tm_utc);

    time_zone = ((t1 - t2) / 3600);

    return time_zone;
}

/**
 * @brief  设置当前系统时区(仅对当前进程有效)
 * @param  time_zone: 输入参数, 需要设置的时区(例如: "CST-8")
 * @return true : 成功
 * @return false: 失败
 */
bool set_local_time_zone(const char *time_zone)
{
    if (NULL == time_zone)
    {
        return false;
    }

    int ret = -1;
    ret = setenv("TZ", time_zone, 1);
    if (0 != ret)
    {
        return false;
    }

    tzset();

    return true;
}

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
bool get_time_str(char *time_buf, const uint8_t gap_flag)
{
    if (NULL == time_buf)
    {
        return false;
    }

    struct timespec time_spec = {0};
    if (-1 == clock_gettime(CLOCK_REALTIME, &time_spec))
    {
        return false;
    }

    struct tm *tm_local = localtime(&time_spec.tv_sec);
    if (NULL == tm_local)
    {
        return false;
    }

    if (0 == gap_flag)
    {
        snprintf(time_buf, 60, "%4d%02d%02d%02d%02d%02d%06ld", (1900 + tm_local->tm_year), (1 + tm_local->tm_mon),
                 tm_local->tm_mday, tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec, (time_spec.tv_nsec / 1000));

        return true;
    }
    else if (1 == gap_flag)
    {
        snprintf(time_buf, 66, "%4d-%02d-%02d %02d:%02d:%02d.%06ld", (1900 + tm_local->tm_year), (1 + tm_local->tm_mon),
                 tm_local->tm_mday, tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec, (time_spec.tv_nsec / 1000));

        return true;
    }
    else if (2 == gap_flag)
    {
        snprintf(time_buf, 66, "%4d-%02d-%02d_%02d-%02d-%02d-%06ld", (1900 + tm_local->tm_year), (1 + tm_local->tm_mon),
                 tm_local->tm_mday, tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec, (time_spec.tv_nsec / 1000));

        return true;
    }

    return false;
}

/**
 * @brief  获取系统当前时间
 * @param  local_time: 输出参数, 获取到的本地时间
 * @return true : 成功
 * @return false: 失败
 */
bool get_local_time(date_time_t *local_time)
{
    if (NULL == local_time)
    {
        return false;
    }

    struct timespec time_spec = {0};
    if (-1 == clock_gettime(CLOCK_REALTIME, &time_spec))
    {
        return false;
    }

    struct tm *tm_local = localtime(&time_spec.tv_sec);
    if (NULL == tm_local)
    {
        return false;
    }

    local_time->tm_year = (tm_local->tm_year + 1900);
    local_time->tm_mon = (tm_local->tm_mon + 1);
    local_time->tm_day = tm_local->tm_mday;
    local_time->tm_hour = tm_local->tm_hour;
    local_time->tm_min = tm_local->tm_min;
    local_time->tm_sec = tm_local->tm_sec;

    return true;
}

/**
 * @brief  设置系统当前时间
 * @param  local_time: 输入参数, 当前时间
 * @return true : 成功
 * @return false: 失败
 */
bool set_local_time(const date_time_t local_time)
{
    int ret = -1;
    time_t t = -1;
    struct tm new_time = {0};

    new_time.tm_sec = local_time.tm_sec;
    new_time.tm_min = local_time.tm_min;
    new_time.tm_hour = local_time.tm_hour;
    new_time.tm_mday = local_time.tm_day;
    new_time.tm_mon = (local_time.tm_mon - 1);
    new_time.tm_year = (local_time.tm_year - 1900);
    new_time.tm_isdst = -1;

    t = mktime(&new_time);
#if __GLIBC_MINOR__ == 31
    struct timespec res;
    res.tv_sec = t;
    clock_settime(CLOCK_REALTIME, &res);
#else
    ret = stime(&t);
#endif
    if (0 != ret)
    {
        return false;
    }

    return true;
}

/**
 * @brief  获取当前毫秒时间
 * @param  msec: 输出参数, 获取到的毫秒时间
 * @return true : 成功
 * @return false: 失败
 */
bool get_current_msec(uint64_t *msec)
{
    if (NULL == msec)
    {
        return false;
    }

    struct timespec time_spec = {0};
    if (-1 == clock_gettime(CLOCK_REALTIME, &time_spec))
    {
        return false;
    }

    *msec = time_spec.tv_sec * 1000 + time_spec.tv_nsec / 1000000;

    return true;
}

/**
 * @brief  获取系统已运行时间
 * @param  running_time: 系统已运行时间
 * @return true : 成功
 * @return false: 失败
 */
bool get_running_time(uint64_t *running_time)
{
    if (NULL == running_time)
    {
        return false;
    }

    struct timespec time_spec = {0};
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &time_spec))
    {
        return false;
    }

    *running_time = time_spec.tv_sec * 1000 + time_spec.tv_nsec / 1000000;

    return true;
}

/**
 * @brief  unix时间戳转换为本地时间
 * @param  local_time: 输出参数, 转换后的本地时间
 * @param  unix_time : 输入参数, unix时间戳
 */
void unix_time_to_local_time(date_time_t *local_time, const uint64_t unix_time)
{
    struct tm *time = NULL;

    // 获取当地时间
    time = localtime((const time_t *)&unix_time);

    local_time->tm_year = (time->tm_year + 1900);
    local_time->tm_mon = (time->tm_mon + 1);
    local_time->tm_day = time->tm_mday;
    local_time->tm_hour = time->tm_hour;
    local_time->tm_min = time->tm_min;
    local_time->tm_sec = time->tm_sec;
}

/**
 * @brief  本地时间转换为unix时间戳
 * @param  unix_time : 输出参数, 转换后的unix时间戳
 * @param  local_time: 输入参数, 本地时间
 */
void local_time_to_unix_time(uint64_t *unix_time, const date_time_t local_time)
{
    struct tm time = {0};

    time.tm_year = (local_time.tm_year - 1900);
    time.tm_mon = (local_time.tm_mon - 1);
    time.tm_mday = local_time.tm_day;
    time.tm_hour = local_time.tm_hour;
    time.tm_min = local_time.tm_min;
    time.tm_sec = local_time.tm_sec;

    *unix_time = mktime(&time);
}

/**
 * @brief  从系统时间设置硬件时钟
 * @param  device: 输入参数, 指定设备(形如: /dev/rtc0)
 */
void set_hardware_clock_from_system_time(const char *device)
{
    if (!device)
    {
        return;
    }

    char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "hwclock -w -f %s", device);
    system(cmd);
}

/**
 * @brief  从硬件时钟设置系统时间
 * @param  device: 输入参数, 指定设备(形如: /dev/rtc0)
 */
void set_system_time_from_hardware_clock(const char *device)
{
    if (!device)
    {
        return;
    }

    char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "hwclock -s -f %s", device);
    system(cmd);
}
