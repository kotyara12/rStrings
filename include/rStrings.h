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

/**
 * Clone a string and allocate a new memory area on the heap
 * */
char* malloc_string(const char *source);

/**
 * Generating a string in heap with formatting
 * */
char* malloc_stringf(const char *format, ...);

/**
 * Generating a heap string containing a textual representation of the date and time
 * */
char* malloc_timestr(const char *format, time_t value);

/**
 * Generation of a truncated name of a topic: prefix + site + / + topic 
 * for example: "/home/heater"
 * 
 * Note: mqttGetTopic0 uses values from project_congig.h
 * 
 * @param perfix - Server topic prefix, may be "/" or "" or "user_XXXX" for some servers
 * @param site - Device location, may be "home" or "village"
 * @param topic - Topic name
 * @return - Pointer to a string in heap. Remember to free it after using the function esp_mqtt_free_string() or free();
 * */
char * mqttGetTopicCustom0(const char *perfix, const char *site, const char *topic);
char * mqttGetTopic0(const char *topic);

/**
 * Generation of a name of a topic: prefix + site + / + device + / + topic
 * for example: "/village/heater/status"
 * 
 * Note: mqttGetTopic1 uses values from project_congig.h
 * 
 * @param perfix - Server topic prefix, may be "/" or "" or "user_XXXX" for some servers
 * @param site - Device location, may be "home" or "village"
 * @param device - Esp device name 
 * @param topic - Topic name
 * @return - Pointer to a string in heap. Remember to free it after using the function esp_mqtt_free_string() or free();
 * */
char * mqttGetTopicCustom1(const char *perfix, const char *site, const char *device, const char *topic);
char * mqttGetTopic1(const char *topic);

/**
 * Generation of a name of a topic: prefix + site + / + device + / + topic1 + / + topic2
 * for example: "/village/heater/bedroom/temperature"
 * 
 * Note: mqttGetTopic1 uses values from project_congig.h
 * 
 * @param perfix - Server topic prefix, may be "/" or "" or "user_XXXX" for some servers
 * @param site - Device location, may be "home" or "village"
 * @param device - Esp device name 
 * @param topic1 - Topic name
 * @param topic2 - Subtopic name
 * @return - Pointer to a string in heap. Remember to free it after using the function esp_mqtt_free_string() or free();
 * */
char * mqttGetTopicCustom2(const char *perfix, const char *site, const char *device, const char *topic1, const char *topic2);
char * mqttGetTopic2(const char *topic1, const char *topic2);

/**
 * Generation of a name of a topic: prefix + site + / + device + / + topic1 + / + topic2 + / + topic3
 * for example: "/village/heater/bedroom/temperature/sensor_status"
 * 
 * Note: mqttGetTopic1 uses values from project_congig.h
 * 
 * @param perfix - Server topic prefix, may be "/" or "" or "user_XXXX" for some servers
 * @param site - Device location, may be "home" or "village"
 * @param device - Ssp device name 
 * @param topic1 - Topic name
 * @param topic2 - Subtopic name
 * @param topic3 - Subsubtopic name
 * @return - Pointer to a string in heap. Remember to free it after using the function esp_mqtt_free_string() or free();
 * */
char * mqttGetTopicCustom3(const char *perfix, const char *site, const char *device, const char *topic1, const char *topic2, const char *topic3);
char * mqttGetTopic3(const char *topic1, const char *topic2, const char *topic3);
char * mqttGetTopic(const char *topic1, const char *topic2, const char *topic3);

#ifdef __cplusplus
}
#endif


#endif // __RSTRINGS32_H__

