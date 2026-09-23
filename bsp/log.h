#ifndef LOG_H
#define LOG_H

#define LOG_INTERFACE   printf
 
#define LOG_LEVEL_NONE   0
#define LOG_LEVEL_ERROR  1
#define LOG_LEVEL_WARN   2
#define LOG_LEVEL_INFO   3

#define LOG_FILE_LEVEL LOG_LEVEL_INFO

#if LOG_FILE_LEVEL >= LOG_LEVEL_ERROR
    #define LOG_ERROR(tag, format, ...)  LOG_INTERFACE("[Error] " tag format "\r\n", ##__VA_ARGS__)
#else
    #define LOG_ERROR(tag, format, ...)  do {} while(0)
#endif

#if LOG_FILE_LEVEL >= LOG_LEVEL_WARN
    #define LOG_WARN(tag, format, ...)  LOG_INTERFACE("[Warn] " tag format "\r\n", ##__VA_ARGS__)
#else
    #define LOG_WARN(tag, format, ...)  do {} while(0)
#endif

#if LOG_FILE_LEVEL >= LOG_LEVEL_INFO
    #define LOG_INFO(tag, format, ...)  LOG_INTERFACE("[Info] " tag format "\r\n", ##__VA_ARGS__)
#else
    #define LOG_INFO(tag, format, ...)  do {} while(0)
#endif

#endif