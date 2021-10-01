#include "rStrings.h"
#include "rLog.h"
#include "project_config.h"
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
  memset(ret, 0, len+1);
  strcpy(ret, source);
  return ret;
}

char * malloc_stringl(const char *source, const uint32_t len) 
{
  char *ret = (char*)malloc(len+1);
  if (!ret) {
    rlog_e(tagHEAP, "Out of memory!");
    // ledSysStateSet(SYSLED_ERROR, false);
    return NULL;
  }
  memset(ret, 0, len+1);
  strncpy(ret, source, len);
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
    memset(ret, 0, len+1);
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
  memset(&buffer, 0, sizeof(buffer));
  localtime_r(&value, &timeinfo);
  strftime(buffer, sizeof(buffer), format, &timeinfo);
  return malloc_string(buffer);
}

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- Create topics ------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

char * mqttGetSubTopic(const char *topic, const char *subtopic)
{
  return malloc_stringf("%s/%s", topic, subtopic);
}

#if defined(CONFIG_MQTT1_LOC_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT1_LOC_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT1_LOC_HEADER_SHORT CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_LOCATION "/" CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_LOCATION
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT1_LOC_HEADER_SHORT CONFIG_MQTT1_LOC_PREFIX
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // PREFIX + DEVICE
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // PREFIX
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_PREFIX
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT1_LOC_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT1_LOC_LOCATION)
    // LOCATION + ...
    #define MQTT1_LOC_HEADER_SHORT CONFIG_MQTT1_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_LOCATION "/" CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // LOCATION
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_LOCATION "/"
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #else
    // ...
    #define MQTT1_LOC_HEADER_SHORT ""
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // DEVICE
      #define MQTT1_LOC_HEADER_FULL CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // EMPTY
      #define MQTT1_LOC_HEADER_FULL ""
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT1_LOC_LOCATION)
#endif // defined(CONFIG_MQTT1_LOC_PREFIX)

#if defined(CONFIG_MQTT2_LOC_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT2_LOC_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT2_LOC_HEADER_SHORT CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_LOCATION "/" CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_LOCATION
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT2_LOC_HEADER_SHORT CONFIG_MQTT2_LOC_PREFIX
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // PREFIX + DEVICE
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // PREFIX
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_PREFIX
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT2_LOC_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT2_LOC_LOCATION)
    // LOCATION + ...
    #define MQTT2_LOC_HEADER_SHORT CONFIG_MQTT2_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_LOCATION "/" CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // LOCATION
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_LOCATION "/"
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #else
    // ...
    #define MQTT2_LOC_HEADER_SHORT ""
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // DEVICE
      #define MQTT2_LOC_HEADER_FULL CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // EMPTY
      #define MQTT2_LOC_HEADER_FULL ""
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT2_LOC_LOCATION)
#endif // defined(CONFIG_MQTT2_LOC_PREFIX)

#if defined(CONFIG_MQTT1_PUB_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT1_PUB_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT1_PUB_HEADER_SHORT CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_LOCATION "/" CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_LOCATION
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT1_PUB_HEADER_SHORT CONFIG_MQTT1_PUB_PREFIX
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // PREFIX + DEVICE
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // PREFIX
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_PREFIX
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT1_PUB_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT1_PUB_LOCATION)
    // LOCATION + ...
    #define MQTT1_PUB_HEADER_SHORT CONFIG_MQTT1_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_LOCATION "/" CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // LOCATION
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_LOCATION "/"
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #else
    // ...
    #define MQTT1_PUB_HEADER_SHORT ""
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // DEVICE
      #define MQTT1_PUB_HEADER_FULL CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // EMPTY
      #define MQTT1_PUB_HEADER_FULL ""
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT1_PUB_LOCATION)
#endif // defined(CONFIG_MQTT1_PUB_PREFIX)

#if defined(CONFIG_MQTT2_PUB_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT2_PUB_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT2_PUB_HEADER_SHORT CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_LOCATION "/" CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_LOCATION
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT2_PUB_HEADER_SHORT CONFIG_MQTT2_PUB_PREFIX
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // PREFIX + DEVICE
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // PREFIX
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_PREFIX
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT2_PUB_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT2_PUB_LOCATION)
    // LOCATION + ...
    #define MQTT2_PUB_HEADER_SHORT CONFIG_MQTT2_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_LOCATION "/" CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // LOCATION
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_LOCATION "/"
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #else
    // ...
    #define MQTT2_PUB_HEADER_SHORT ""
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // DEVICE
      #define MQTT2_PUB_HEADER_FULL CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // EMPTY
      #define MQTT2_PUB_HEADER_FULL ""
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT2_PUB_LOCATION)
#endif // defined(CONFIG_MQTT2_PUB_PREFIX)

char * mqttGetTopic0(const bool primary, const bool local, const char *topic)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_LOC_HEADER_SHORT, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_LOC_HEADER_SHORT, topic);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_PUB_HEADER_SHORT, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_PUB_HEADER_SHORT, topic);
    };
  };
}

char * mqttGetTopic1(const bool primary, const bool local, const char *topic)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_LOC_HEADER_FULL, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_LOC_HEADER_FULL, topic);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_PUB_HEADER_FULL, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_PUB_HEADER_FULL, topic);
    };
  };
}

char * mqttGetTopic2(const bool primary, const bool local, const char *topic1, const char *topic2)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s", MQTT1_LOC_HEADER_FULL, topic1, topic2);
    } else {
      return malloc_stringf("%s%s/%s", MQTT2_LOC_HEADER_FULL, topic1, topic2);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s", MQTT1_PUB_HEADER_FULL, topic1, topic2);
    } else {
      return malloc_stringf("%s%s/%s", MQTT2_PUB_HEADER_FULL, topic1, topic2);
    };
  };
}

char * mqttGetTopic3(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s", MQTT1_LOC_HEADER_FULL, topic1, topic2, topic3);
    } else {
      return malloc_stringf("%s%s/%s/%s", MQTT2_LOC_HEADER_FULL, topic1, topic2, topic3);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s", MQTT1_PUB_HEADER_FULL, topic1, topic2, topic3);
    } else {
      return malloc_stringf("%s%s/%s/%s", MQTT2_PUB_HEADER_FULL, topic1, topic2, topic3);
    };
  };
}

char * mqttGetTopic(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3)
{
  if (topic3) {
    return mqttGetTopic3(primary, local, topic1, topic2, topic3);
  }
  else if (topic2) {
    return mqttGetTopic2(primary, local, topic1, topic2);
  }
  else if (topic1) {
    return mqttGetTopic1(primary, local, topic1);
  }
  else {
    return nullptr;
  }
}

