//
// Created by erpang on 6/7/23.
//

#ifndef MINI_REDIS_LOG_H
#define MINI_REDIS_LOG_H

#endif //MINI_REDIS_LOG_H

#include <stdio.h>

/*
显示：0(默认)、1(粗体/高亮)、22(非粗体)、4(单条下划线)、24(无下划线)、5(闪烁)、25(无闪烁)、7(反显、翻转前景色和背景色)、27(无反显)
颜色值：0(黑)、1(红)、2(绿)、 3(黄)、4(蓝)、5(紫)、6(深绿)、7(白)
颜色分为背景色和字体色，30~39（30+颜色值）用来设置字体色（前景色），40~49（40+颜色值）设置背景（背景色）：如31表示前景色为红色，41表示背景色为红色。
*/
#define COLOR_NONE                  "\033[0;m"
#define RED                         "\033[0;31m"
#define LIGHT_RED                   "\033[1;31m"  //红色高亮
#define LIGHT_RED_INV               "\033[5;7;31m"  //红色高亮，并反白显示，字体闪烁
#define GREEN                       "\033[0;32m"
#define LIGHT_GREEN                 "\033[1;32m"
#define BLUE                        "\033[0;34m"
#define LIGHT_BLUE                  "\033[1;34m" //蓝色高亮
#define DARY_GRAY                   "\033[1;30m"
#define CYAN                        "\033[0;36m"
#define LIGHT_CYAN                  "\033[1;36m"
#define PURPLE                      "\033[0;35m"
#define LIGHT_PURPLE                "\033[1;35m"
#define YELLOW                      "\033[0;33m"
#define LIGHT_YELLOW                "\033[1;33m" //黄色高亮
#define WHITE                                   "\033[0;37m"
#define LIGHT_WHITE                 "\033[1;37m" //白色高亮

/*
日志级别定义，数值越小，级别越高
*/
#define LOGGER_FATAL_LEVEL          1   //致命错误，不可恢复
#define LOGGER_ERROR_LEVEL          2   //一般错误，可恢复
#define LOGGER_WARN_LEVEL           3   //警告
#define LOGGER_INFO_LEVEL           4   //信息
#define LOGGER_DEBUG_LEVEL          5   //调试

/*
当前日志级别配置
*/
#define LOGGER_CFG_LEVEL            LOGGER_DEBUG_LEVEL

/*
根据配置的日志级别输出日志
*/
#define logger_level_printf(level, fmt, arg...)\
    do {\
        if (level <= LOGGER_CFG_LEVEL) {\
            printf("[%s,%d] "fmt, __FILE__, __LINE__, ##arg);\
        }\
    } while(0)

#define logger_fatal(fmt, arg...)   logger_level_printf(LOGGER_FATAL_LEVEL, LIGHT_RED_INV fmt COLOR_NONE, ##arg)
#define logger_error(fmt, arg...)   logger_level_printf(LOGGER_ERROR_LEVEL, LIGHT_RED fmt COLOR_NONE, ##arg)
#define logger_warn(fmt, arg...)    logger_level_printf(LOGGER_WARN_LEVEL, LIGHT_YELLOW fmt COLOR_NONE, ##arg)
#define logger_info(fmt, arg...)    logger_level_printf(LOGGER_INFO_LEVEL, LIGHT_BLUE fmt COLOR_NONE, ##arg)
#define logger_debug(fmt, arg...)   logger_level_printf(LOGGER_DEBUG_LEVEL, LIGHT_WHITE fmt COLOR_NONE, ##arg)