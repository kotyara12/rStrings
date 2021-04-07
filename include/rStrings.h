/* 
   EN: Dynamically allocating strings on the heap (ESP-IDF)
   RU: Динамическое выделение строк в куче и работа с ними (ESP-IDF)
   --------------------------
   (с) 2021 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RSTRINGS32_H__
#define __RSTRINGS32_H__

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

char* malloc_string(const char *source);
char* malloc_stringf(const char *format, ...);
char* malloc_timestr(const char *format, time_t value);

#ifdef __cplusplus
}
#endif


#endif // __RSTRINGS32_H__

