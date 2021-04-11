#include "rStrings.h"
#include "rLog.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static const char * tagHEAP = "OUT OF MEMORY";

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- Format strings -----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

char * malloc_string(const char *source) 
{
  uint32_t len = strlen(source);
  char *ret = (char*)malloc(len+1);
  if (!ret) {
    rlog_e(tagHEAP, "Out of memory!");
    // ledSysStateSet(SYSLED_ERROR, false);
    return NULL;
  }
  strcpy(ret, source);
  return ret;
}

char * malloc_stringf(const char *format, ...) 
{
  uint32_t len;
  va_list args;
  char *ret;

  // get the list of arguments
  va_start(args, format);
  // calculate length of resulting string
  len = vsnprintf(NULL, 0, format, args);
  // allocate memory for string
  ret = (char*)malloc(len+1);
  if (ret) {
    // get resulting string into buffer
    vsnprintf(ret, len+1, format, args);
  } else {
    rlog_e(tagHEAP, "Out of memory!");
    // ledSysStateSet(SYSLED_ERROR, false);
  };
  va_end(args);
  return ret;
}

char * malloc_timestr(const char *format, time_t value)
{
  struct tm timeinfo;
  char buffer[64];
  localtime_r(&value, &timeinfo);
  strftime(buffer, sizeof(buffer), format, &timeinfo);
  return malloc_string(buffer);
}

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- Create topics ------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

#define MQTT_TOPIC_TEMPLATE_0 "%s%s/%s"          // prefix + site + / + topic (etc: "/village/heater")
#define MQTT_TOPIC_TEMPLATE_1 "%s%s/%s/%s"       // prefix + site + / + device + / + topic (etc: "/village/heater/status")
#define MQTT_TOPIC_TEMPLATE_2 "%s%s/%s/%s/%s"    // prefix + site + / + device + / + topic + / + subtopic (etc: "/village/heater/bedroom/temperature")
#define MQTT_TOPIC_TEMPLATE_3 "%s%s/%s/%s/%s/%s" // prefix + site + / + device + / + topic1 + / + topic2 + / + topic3 (etc: "/village/heater/bedroom/temperature/sensor_status")

char * mqttGetTopicCustom0(const char *perfix, const char *site, const char *topic)
{
  return malloc_stringf(MQTT_TOPIC_TEMPLATE_0, perfix, site, topic);
}

char * mqttGetTopic0(const char *topic)
{
  #ifdef CONFIG_MQTT_PREFIX
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_0, CONFIG_MQTT_PREFIX, CONFIG_MQTT_SITE, topic);
  #else
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_0, "", CONFIG_MQTT_SITE, topic);
  #endif
}

char * mqttGetTopicCustom1(const char *perfix, const char *site, const char *device, const char *topic)
{
  return malloc_stringf(MQTT_TOPIC_TEMPLATE_1, perfix, site, device, topic);
}

char * mqttGetTopic1(const char *topic)
{
  #ifdef CONFIG_MQTT_PREFIX
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_1, CONFIG_MQTT_PREFIX, CONFIG_MQTT_SITE, CONFIG_MQTT_DEVICE, topic);
  #else
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_1, "", CONFIG_MQTT_PREFIX, CONFIG_MQTT_SITE, CONFIG_MQTT_DEVICE, topic);
  #endif
}

char * mqttGetTopicCustom2(const char *perfix, const char *site, const char *device, const char *topic1, const char *topic2)
{
  return malloc_stringf(MQTT_TOPIC_TEMPLATE_2, perfix, site, device, topic1, topic2);
}

char * mqttGetTopic2(const char *topic1, const char *topic2)
{
  #ifdef CONFIG_MQTT_PREFIX
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_2, CONFIG_MQTT_PREFIX, CONFIG_MQTT_SITE, CONFIG_MQTT_DEVICE, topic1, topic2);
  #else
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_2, "", CONFIG_MQTT_PREFIX, CONFIG_MQTT_SITE, CONFIG_MQTT_DEVICE, topic1, topic2);
  #endif
}

char * mqttGetTopicCustom3(const char *perfix, const char *site, const char *device, const char *topic1, const char *topic2, const char *topic3)
{
  return malloc_stringf(MQTT_TOPIC_TEMPLATE_3, perfix, site, device, topic1, topic2, topic3);
}

char * mqttGetTopic3(const char *topic1, const char *topic2, const char *topic3)
{
  #ifdef CONFIG_MQTT_PREFIX
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_3, CONFIG_MQTT_PREFIX, CONFIG_MQTT_SITE, CONFIG_MQTT_DEVICE, topic1, topic2, topic3);
  #else
    return malloc_stringf(MQTT_TOPIC_TEMPLATE_3, "", CONFIG_MQTT_SITE, CONFIG_MQTT_DEVICE, topic1, topic2, topic3);
  #endif
}

char * mqttGetTopic(const char *topic1, const char *topic2, const char *topic3)
{
  if (topic3) {
    return mqttGetTopic3(topic1, topic2, topic3);
  }
  else if (topic2) {
    return mqttGetTopic2(topic1, topic2);
  }
  else if (topic1) {
    return mqttGetTopic1(topic1);
  }
  else {
    return mqttGetTopic0(CONFIG_MQTT_DEVICE);
  }
}

