/* 
   EN: Dynamically allocating strings on the heap (ESP-IDF)
   RU: Динамическое выделение строк в куче и работа с ними (ESP-IDF)
   --------------------------
   (с) 2021 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __R_STRINGS_H__
#define __R_STRINGS_H__

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Clone a string and allocate a new memory area on the heap
 * */
char* malloc_string(const char *source);

/**
 * Clone a string and allocate a new memory area on the heap
 * */
char* malloc_stringl(const char *source, const uint32_t len);

/**
 * Generating a string in heap with formatting
 * */
char* malloc_stringf(const char *format, ...);
uint16_t format_string(char* buffer, uint16_t buffer_size, const char *format, ...);

/**
 * Concatenation of two strings into one, with removal from the heap of constituent parts
 * */
char * concat_strings(char * part1, char * part2);
char * concat_strings_div(char * part1, char * part2, const char* divider);

/**
 * Converting 64-bit integers to a string
 * */
char* _i64toa(int64_t value, char* buffer, uint8_t radix);
char* _ui64toa(uint64_t value, char* buffer, uint8_t radix);

/**
 * Generating a heap string containing a textual representation of the date and time
 * */
size_t time2str(const char *format, time_t *value, char* buffer, size_t buffer_size);
size_t time2str_empty(const char *format, time_t *value, char* buffer, size_t buffer_size);
// char* malloc_timestr(const char *format, time_t value);
// char* malloc_timestr_empty(const char *format, time_t value);

/**
 * Generating a heap string containing a textual representation of a time interval in hours, minutes, and seconds
 * */
char * malloc_timespan_hms(time_t value);

/**
 * Generating a heap string containing a textual representation of a time interval in days, hours, minutes, and seconds
 * */
char * malloc_timespan_dhms(time_t value);

/**
 * Adding one or more parts to the current topic
 * 
 * @param topic - Topic name
 * @param subtopic - Subtopic name 
 * @return - Pointer to a string in heap. Remember to free it after using the function esp_mqtt_free_string() or free();
 * */
char * mqttGetSubTopic(const char *topic, const char *subtopic);

/**
 * Generation of a name of a topic: prefix + location + / + topic 
 * for example: "/home/heater"
 * 
 * Note: mqttGetTopicLocation uses values from project_congig.h
 * 
 * @param primary - Primary or backup MQTT broker
 * @param local - Local (hidden) topic (available only within this location)
 * @param topic - Topic name
 * @param topic2 - Subtopic name
 * @param topic3 - Subsubtopic name
 * @return - Pointer to a string in heap. Remember to free it after using the function free(...);
 * */
char * mqttGetTopicLocation1(const bool primary, const bool local, const char *topic);
char * mqttGetTopicLocation2(const bool primary, const bool local, const char *topic1, const char *topic2);
char * mqttGetTopicLocation3(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3);
char * mqttGetTopicLocation(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3);

/**
 * Generation of a name of a topic: prefix + location + / + special + / + topic 
 * for example: "/home/custom/heater"
 * 
 * Note: mqttGetTopicSpecial uses values from project_congig.h
 * 
 * @param primary - Primary or backup MQTT broker
 * @param local - Local (hidden) topic (available only within this location)
 * @param device - Special (device) name
 * @param topic - Topic name
 * @param topic2 - Subtopic name
 * @param topic3 - Subsubtopic name
 * @param topic4 - Subsubsubtopic name
 * @return - Pointer to a string in heap. Remember to free it after using the function free(...);
 * */
char * mqttGetTopicSpecial1(const bool primary, const bool local, const char *special, const char *topic);
char * mqttGetTopicSpecial2(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2);
char * mqttGetTopicSpecial3(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3);
char * mqttGetTopicSpecial4(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3, const char *topic4);
char * mqttGetTopicSpecial5(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3, const char *topic4, const char *topic5);
char * mqttGetTopicSpecial(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3);

/**
 * Generation of a name of a topic: prefix + location + / + device + / + topic 
 * for example: "/home/device/heater"
 * 
 * Note: mqttGetTopicDevice uses values from project_congig.h
 * 
 * @param primary - Primary or backup MQTT broker
 * @param local - Local (hidden) topic (available only within this location)
 * @param topic - Topic name
 * @param topic2 - Subtopic name
 * @param topic3 - Subsubtopic name
 * @param topic4 - Subsubsubtopic name
 * @param topic5 - Subsubsubsubtopic name
 * @return - Pointer to a string in heap. Remember to free it after using the function free(...);
 * */
char * mqttGetTopicDevice1(const bool primary, const bool local, const char *topic);
char * mqttGetTopicDevice2(const bool primary, const bool local, const char *topic1, const char *topic2);
char * mqttGetTopicDevice3(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3);
char * mqttGetTopicDevice4(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3, const char *topic4);
char * mqttGetTopicDevice5(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3, const char *topic4, const char *topic5);
char * mqttGetTopicDevice(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3);

#ifdef __cplusplus
}
#endif

#endif // __R_STRINGS_H__

