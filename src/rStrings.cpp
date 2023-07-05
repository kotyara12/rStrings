#include "rStrings.h"
#include "project_config.h"
#include "def_consts.h"
#include "rLog.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#if defined(__has_include) && __has_include("reEsp32.h")
  #include "reEsp32.h"
  #define USE_ESP_MALLOC 1
#else
  #define USE_ESP_MALLOC 0
#endif

#if CONFIG_RLOG_PROJECT_LEVEL > RLOG_LEVEL_NONE
static const char * tagHEAP = "OUT OF MEMORY";
static const char * tagFMTS = "FORMAT";
#endif // CONFIG_RLOG_PROJECT_LEVEL

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- Format strings -----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

char * malloc_string(const char *source) 
{
  if (source) {
    uint32_t len = strlen(source);
    #if USE_ESP_MALLOC
      char *ret = (char*)esp_malloc(len+1);
    #else
      char *ret = (char*)malloc(len+1);
    #endif
    if (ret == nullptr) {
      rlog_e(tagHEAP, "Failed to create string: out of memory!");
      return nullptr;
    }
    memset(ret, 0, len+1);
    strcpy(ret, source);
    return ret;
  };
  return nullptr;
}

char * malloc_stringl(const char *source, const uint32_t len) 
{
  if (source) {
    #if USE_ESP_MALLOC
      char *ret = (char*)esp_malloc(len+1);
    #else
      char *ret = (char*)malloc(len+1);
    #endif
    if (ret == nullptr) {
      rlog_e(tagHEAP, "Failed to create string: out of memory!");
      return nullptr;
    }
    memset(ret, 0, len+1);
    strncpy(ret, source, len);
    return ret;
  };
  return nullptr;
}

char * malloc_stringf(const char *format, ...) 
{
  char *ret = nullptr;
  if (format != nullptr) {
    // get the list of arguments
    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);
    // calculate length of resulting string
    int len = vsnprintf(nullptr, 0, format, args1);
    va_end(args1);
    // allocate memory for string
    if (len > 0) {
      #if USE_ESP_MALLOC
        ret = (char*)esp_malloc(len+1);
      #else
        ret = (char*)malloc(len+1);
      #endif
      if (ret != nullptr) {
        memset(ret, 0, len+1);
        vsnprintf(ret, len+1, format, args2);
      } else {
        rlog_e(tagHEAP, "Failed to format string: out of memory!");
      };
    };
    va_end(args2);
  };
  return ret;
}

uint16_t format_string(char* buffer, uint16_t buffer_size, const char *format, ...)
{
  uint16_t ret = 0;
  if (buffer && format) {
    memset(buffer, 0, buffer_size);
    // get the list of arguments
    va_list args;
    va_start(args, format);
    uint16_t len = vsnprintf(nullptr, 0, format, args);
    // format string
    if (len+1 > buffer_size) {
      ret = -len;
      rlog_e(tagFMTS, "Buffer %d bytes too small to hold formatted string, %d bytes needed", buffer_size, len+1);
    };
    ret = vsnprintf(buffer, buffer_size, format, args);
    va_end(args);
  };
  return ret;
}

char* _i64toa(int64_t value, char* buffer, uint8_t radix) 
{
  // Check radix
  if (radix < 2 || radix > 16) { 
    *buffer = '\0'; 
    return buffer; 
  };

  uint8_t negative, digit;
  uint64_t val;
  char *pos = buffer, *pos1 = buffer, tmp;

  if (value < 0 && radix == 10) {
    negative = 1;
    val = - value;
  } else {
    negative = 0;
    val = value;
  };

  do {
    digit = val % radix;
    val = val / radix;
    if (digit < 10) {
      *pos++ = '0' + digit;
    } else {
      *pos++ = 'a' + digit - 10;
    };
  } while (val != 0L);

  if (negative) *pos++ = '-'; 
  
  *pos-- = '\0';

  while(pos1 < pos) {
    tmp = *pos;
    *pos--= *pos1;
    *pos1++ = tmp;
  };

  return buffer;
}

char* _ui64toa(uint64_t value, char* buffer, uint8_t radix) 
{
  // Check radix
  if (radix < 2 || radix > 16) { 
    *buffer = '\0'; 
    return buffer; 
  };

  uint8_t digit;
  uint64_t val;
  char *pos = buffer, *pos1 = buffer, tmp;

  val = value;
  do {
    digit = val % radix;
    val = val / radix;
    if (digit < 10) {
      *pos++ = '0' + digit;
    } else {
      *pos++ = 'a' + digit - 10;
    };
  } while (val != 0L);

  *pos-- = '\0';

  while(pos1 < pos) {
    tmp = *pos;
    *pos--= *pos1;
    *pos1++ = tmp;
  };

  return buffer;
}

size_t time2str(const char *format, time_t *value, char* buffer, size_t buffer_size)
{
  if ((buffer == nullptr) || (value == nullptr) || (buffer_size == 0)) {
    return 0;
  };
  memset(buffer, 0, buffer_size);
  struct tm timeinfo;
  localtime_r(value, &timeinfo);
  return strftime(buffer, buffer_size, format, &timeinfo);
}

size_t time2str_empty(const char *format, time_t *value, char* buffer, size_t buffer_size)
{
  if ((buffer == nullptr) || (value == nullptr) || (buffer_size == 0)) {
    return 0;
  };
  memset(buffer, 0, buffer_size);
  if (*value > 0) {
    struct tm timeinfo;
    localtime_r(value, &timeinfo);
    return strftime(buffer, buffer_size, format, &timeinfo);
  } else {
    strcpy(buffer, CONFIG_FORMAT_EMPTY_DATETIME);
    return strlen(CONFIG_FORMAT_EMPTY_DATETIME);
  };
}

char * malloc_timestr(const char *format, time_t value)
{
  struct tm timeinfo;
  localtime_r(&value, &timeinfo);
  char buffer[CONFIG_FORMAT_STRFTIME_BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));
  strftime(buffer, sizeof(buffer), format, &timeinfo);
  return malloc_string(buffer);
}

char * malloc_timestr_empty(const char *format, time_t value)
{
  if (value > 0) {
    struct tm timeinfo;
    localtime_r(&value, &timeinfo);
    char buffer[CONFIG_FORMAT_STRFTIME_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    strftime(buffer, sizeof(buffer), format, &timeinfo);
    return malloc_string(buffer);
  } else {
    return malloc_string(CONFIG_FORMAT_EMPTY_DATETIME);
  }
}

char * malloc_timespan_hms(time_t value)
{
  uint16_t h = value / 3600;
  uint16_t m = value % 3600 / 60;
  uint16_t s = value % 3600 % 60;

  return malloc_stringf("%.2d:%.2d:%.2d", h, m, s);
}

char * malloc_timespan_dhms(time_t value)
{
  uint16_t d = value / 86400;
  uint16_t h = value % 86400 / 3600;
  uint16_t m = value % 86400 % 3600 / 60;
  uint16_t s = value % 86400 % 3600 % 60;

  return malloc_stringf("%d.%.2d:%.2d:%.2d", d, h, m, s);
}

char * concat_strings(char * part1, char * part2)
{
  char * ret = nullptr;
  if (part1) {
    if (part2) {
      ret = malloc_stringf("%s%s", part1, part2);
      free(part1);
      free(part2);
    } else {
      ret = part1;
    };
  } else {
    ret = part2;
  };
  return ret;
}

char * concat_strings_div(char * part1, char * part2, const char* divider)
{
  char * ret = nullptr;
  if (part1) {
    if (part2) {
      ret = malloc_stringf("%s%s%s", part1, divider, part2);
      free(part1);
      free(part2);
    } else {
      ret = part1;
    };
  } else {
    ret = part2;
  };
  return ret;
}

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- Create topics ------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

char * mqttGetSubTopic(const char *topic, const char *subtopic)
{
  return malloc_stringf("%s/%s", topic, subtopic);
}

// 
// MQTT1 LOCAL
// 
#if defined(CONFIG_MQTT1_LOC_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT1_LOC_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT1_LOC_HEADER_LOCATION CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_LOCATION "/" CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_LOCATION
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT1_LOC_HEADER_LOCATION CONFIG_MQTT1_LOC_PREFIX
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // PREFIX + DEVICE
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_PREFIX CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // PREFIX
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_PREFIX
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT1_LOC_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT1_LOC_LOCATION)
    // LOCATION + ...
    #define MQTT1_LOC_HEADER_LOCATION CONFIG_MQTT1_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_LOCATION "/" CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // LOCATION
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_LOCATION "/"
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #else
    // ...
    #define MQTT1_LOC_HEADER_LOCATION ""
    #if defined(CONFIG_MQTT1_LOC_DEVICE)
      // DEVICE
      #define MQTT1_LOC_HEADER_DEVICE CONFIG_MQTT1_LOC_DEVICE "/"
    #else
      // EMPTY
      #define MQTT1_LOC_HEADER_DEVICE ""
    #endif // defined(CONFIG_MQTT1_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT1_LOC_LOCATION)
#endif // defined(CONFIG_MQTT1_LOC_PREFIX)

// 
// MQTT2 LOCAL
// 
#if defined(CONFIG_MQTT2_LOC_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT2_LOC_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT2_LOC_HEADER_LOCATION CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_LOCATION "/" CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_LOCATION
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT2_LOC_HEADER_LOCATION CONFIG_MQTT2_LOC_PREFIX
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // PREFIX + DEVICE
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_PREFIX CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // PREFIX
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_PREFIX
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT2_LOC_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT2_LOC_LOCATION)
    // LOCATION + ...
    #define MQTT2_LOC_HEADER_LOCATION CONFIG_MQTT2_LOC_LOCATION "/"
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_LOCATION "/" CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // LOCATION
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_LOCATION "/"
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #else
    // ...
    #define MQTT2_LOC_HEADER_LOCATION ""
    #if defined(CONFIG_MQTT2_LOC_DEVICE)
      // DEVICE
      #define MQTT2_LOC_HEADER_DEVICE CONFIG_MQTT2_LOC_DEVICE "/"
    #else
      // EMPTY
      #define MQTT2_LOC_HEADER_DEVICE ""
    #endif // defined(CONFIG_MQTT2_LOC_DEVICE)
  #endif // defined(CONFIG_MQTT2_LOC_LOCATION)
#endif // defined(CONFIG_MQTT2_LOC_PREFIX)

// 
// MQTT1 PUBLIC
// 
#if defined(CONFIG_MQTT1_PUB_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT1_PUB_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT1_PUB_HEADER_LOCATION CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_LOCATION "/" CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_LOCATION
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT1_PUB_HEADER_LOCATION CONFIG_MQTT1_PUB_PREFIX
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // PREFIX + DEVICE
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_PREFIX CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // PREFIX
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_PREFIX
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT1_PUB_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT1_PUB_LOCATION)
    // LOCATION + ...
    #define MQTT1_PUB_HEADER_LOCATION CONFIG_MQTT1_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_LOCATION "/" CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // LOCATION
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_LOCATION "/"
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #else
    // ...
    #define MQTT1_PUB_HEADER_LOCATION ""
    #if defined(CONFIG_MQTT1_PUB_DEVICE)
      // DEVICE
      #define MQTT1_PUB_HEADER_DEVICE CONFIG_MQTT1_PUB_DEVICE "/"
    #else
      // EMPTY
      #define MQTT1_PUB_HEADER_DEVICE ""
    #endif // defined(CONFIG_MQTT1_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT1_PUB_LOCATION)
#endif // defined(CONFIG_MQTT1_PUB_PREFIX)

// 
// MQTT2 PUBLIC
// 
#if defined(CONFIG_MQTT2_PUB_PREFIX)
  // PREFIX + ...
  #if defined(CONFIG_MQTT2_PUB_LOCATION)
    // PREFIX + LOCATION + ...
    #define MQTT2_PUB_HEADER_LOCATION CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // PREFIX + LOCATION + "/" + DEVICE + "/"
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_LOCATION "/" CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // PREFIX + LOCATION + "/"
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_LOCATION
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #else
    // PREFIX + ...
    #define MQTT2_PUB_HEADER_LOCATION CONFIG_MQTT2_PUB_PREFIX
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // PREFIX + DEVICE
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_PREFIX CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // PREFIX
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_PREFIX
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT2_PUB_LOCATION)
#else
  // ...
  #if defined(CONFIG_MQTT2_PUB_LOCATION)
    // LOCATION + ...
    #define MQTT2_PUB_HEADER_LOCATION CONFIG_MQTT2_PUB_LOCATION "/"
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // LOCATION + "/" + DEVICE
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_LOCATION "/" CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // LOCATION
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_LOCATION "/"
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #else
    // ...
    #define MQTT2_PUB_HEADER_LOCATION ""
    #if defined(CONFIG_MQTT2_PUB_DEVICE)
      // DEVICE
      #define MQTT2_PUB_HEADER_DEVICE CONFIG_MQTT2_PUB_DEVICE "/"
    #else
      // EMPTY
      #define MQTT2_PUB_HEADER_DEVICE ""
    #endif // defined(CONFIG_MQTT2_PUB_DEVICE)
  #endif // defined(CONFIG_MQTT2_PUB_LOCATION)
#endif // defined(CONFIG_MQTT2_PUB_PREFIX)

// Generation of a name of a topic: prefix + location + / + topic 
char * mqttGetTopicLocation1(const bool primary, const bool local, const char *topic)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_LOC_HEADER_LOCATION, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_LOC_HEADER_LOCATION, topic);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_PUB_HEADER_LOCATION, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_PUB_HEADER_LOCATION, topic);
    };
  };
}

char * mqttGetTopicLocation2(const bool primary, const bool local, const char *topic1, const char *topic2)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s", MQTT1_LOC_HEADER_LOCATION, topic1, topic2);
    } else {
      return malloc_stringf("%s%s/%s", MQTT2_LOC_HEADER_LOCATION, topic1, topic2);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s", MQTT1_PUB_HEADER_LOCATION, topic1, topic2);
    } else {
      return malloc_stringf("%s%s/%s", MQTT2_PUB_HEADER_LOCATION, topic1, topic2);
    };
  };
}

char * mqttGetTopicLocation3(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, topic1, topic2, topic3);
    } else {
      return malloc_stringf("%s%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, topic1, topic2, topic3);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, topic1, topic2, topic3);
    } else {
      return malloc_stringf("%s%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, topic1, topic2, topic3);
    };
  };
}

char * mqttGetTopicLocation(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3)
{
  if (topic3) {
    return mqttGetTopicLocation3(primary, local, topic1, topic2, topic3);
  }
  else if (topic2) {
    return mqttGetTopicLocation2(primary, local, topic1, topic2);
  }
  else if (topic1) {
    return mqttGetTopicLocation1(primary, local, topic1);
  }
  else {
    return nullptr;
  }
}

// Generation of a name of a topic: prefix + location + / + special + / + topic 
char * mqttGetTopicSpecial1(const bool primary, const bool local, const char *special, const char *topic)
{
  if (special) {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s", MQTT1_LOC_HEADER_LOCATION, special, topic);
      } else {
        return malloc_stringf("%s%s/%s", MQTT2_LOC_HEADER_LOCATION, special, topic);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s", MQTT1_PUB_HEADER_LOCATION, special, topic);
      } else {
        return malloc_stringf("%s%s/%s", MQTT2_PUB_HEADER_LOCATION, special, topic);
      };
    };
  } else {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s", MQTT1_LOC_HEADER_LOCATION, topic);
      } else {
        return malloc_stringf("%s%s", MQTT2_LOC_HEADER_LOCATION, topic);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s", MQTT1_PUB_HEADER_LOCATION, topic);
      } else {
        return malloc_stringf("%s%s", MQTT2_PUB_HEADER_LOCATION, topic);
      };
    };
  };
}

char * mqttGetTopicSpecial2(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2)
{
  if (special) {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, special, topic1, topic2);
      } else {
        return malloc_stringf("%s%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, special, topic1, topic2);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, special, topic1, topic2);
      } else {
        return malloc_stringf("%s%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, special, topic1, topic2);
      };
    };
  } else {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s", MQTT1_LOC_HEADER_LOCATION, topic1, topic2);
      } else {
        return malloc_stringf("%s%s/%s", MQTT2_LOC_HEADER_LOCATION, topic1, topic2);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s", MQTT1_PUB_HEADER_LOCATION, topic1, topic2);
      } else {
        return malloc_stringf("%s%s/%s", MQTT2_PUB_HEADER_LOCATION, topic1, topic2);
      };
    };
  };
}

char * mqttGetTopicSpecial3(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3)
{
  if (special) {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, special, topic1, topic2, topic3);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, special, topic1, topic2, topic3);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, special, topic1, topic2, topic3);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, special, topic1, topic2, topic3);
      };
    };
  } else {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, topic1, topic2, topic3);
      } else {
        return malloc_stringf("%s%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, topic1, topic2, topic3);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, topic1, topic2, topic3);
      } else {
        return malloc_stringf("%s%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, topic1, topic2, topic3);
      };
    };
  };
}

char * mqttGetTopicSpecial4(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3, const char *topic4)
{
  if (special) {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, special, topic1, topic2, topic3, topic4);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, special, topic1, topic2, topic3, topic4);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, special, topic1, topic2, topic3, topic4);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, special, topic1, topic2, topic3, topic4);
      };
    };
  } else {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, topic1, topic2, topic3, topic4);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, topic1, topic2, topic3, topic4);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, topic1, topic2, topic3, topic4);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, topic1, topic2, topic3, topic4);
      };
    };
  };
}

char * mqttGetTopicSpecial5(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3, const char *topic4, const char *topic5)
{
  if (special) {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, special, topic1, topic2, topic3, topic4, topic5);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, special, topic1, topic2, topic3, topic4, topic5);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, special, topic1, topic2, topic3, topic4, topic5);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, special, topic1, topic2, topic3, topic4, topic5);
      };
    };
  } else {
    if (local) {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT1_LOC_HEADER_LOCATION, topic1, topic2, topic3, topic4, topic5);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT2_LOC_HEADER_LOCATION, topic1, topic2, topic3, topic4, topic5);
      };
    } else {
      if (primary) {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT1_PUB_HEADER_LOCATION, topic1, topic2, topic3, topic4, topic5);
      } else {
        return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT2_PUB_HEADER_LOCATION, topic1, topic2, topic3, topic4, topic5);
      };
    };
  };
}

char * mqttGetTopicSpecial(const bool primary, const bool local, const char *special, const char *topic1, const char *topic2, const char *topic3)
{
  if (topic3) {
    return mqttGetTopicSpecial3(primary, local, special, topic1, topic2, topic3);
  }
  else if (topic2) {
    return mqttGetTopicSpecial2(primary, local, special, topic1, topic2);
  }
  else if (topic1) {
    return mqttGetTopicSpecial1(primary, local, special, topic1);
  }
  else {
    return nullptr;
  }
}

// Generation of a name of a topic: prefix + location + / + device + / + topic 
char * mqttGetTopicDevice1(const bool primary, const bool local, const char *topic)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_LOC_HEADER_DEVICE, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_LOC_HEADER_DEVICE, topic);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s", MQTT1_PUB_HEADER_DEVICE, topic);
    } else {
      return malloc_stringf("%s%s", MQTT2_PUB_HEADER_DEVICE, topic);
    };
  };
}

char * mqttGetTopicDevice2(const bool primary, const bool local, const char *topic1, const char *topic2)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s", MQTT1_LOC_HEADER_DEVICE, topic1, topic2);
    } else {
      return malloc_stringf("%s%s/%s", MQTT2_LOC_HEADER_DEVICE, topic1, topic2);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s", MQTT1_PUB_HEADER_DEVICE, topic1, topic2);
    } else {
      return malloc_stringf("%s%s/%s", MQTT2_PUB_HEADER_DEVICE, topic1, topic2);
    };
  };
}

char * mqttGetTopicDevice3(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s", MQTT1_LOC_HEADER_DEVICE, topic1, topic2, topic3);
    } else {
      return malloc_stringf("%s%s/%s/%s", MQTT2_LOC_HEADER_DEVICE, topic1, topic2, topic3);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s", MQTT1_PUB_HEADER_DEVICE, topic1, topic2, topic3);
    } else {
      return malloc_stringf("%s%s/%s/%s", MQTT2_PUB_HEADER_DEVICE, topic1, topic2, topic3);
    };
  };
}

char * mqttGetTopicDevice4(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3, const char *topic4)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s/%s", MQTT1_LOC_HEADER_DEVICE, topic1, topic2, topic3, topic4);
    } else {
      return malloc_stringf("%s%s/%s/%s/%s", MQTT2_LOC_HEADER_DEVICE, topic1, topic2, topic3, topic4);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s/%s", MQTT1_PUB_HEADER_DEVICE, topic1, topic2, topic3, topic4);
    } else {
      return malloc_stringf("%s%s/%s/%s/%s", MQTT2_PUB_HEADER_DEVICE, topic1, topic2, topic3, topic4);
    };
  };
}

char * mqttGetTopicDevice5(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3, const char *topic4, const char *topic5)
{
  if (local) {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT1_LOC_HEADER_DEVICE, topic1, topic2, topic3, topic4, topic5);
    } else {
      return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT2_LOC_HEADER_DEVICE, topic1, topic2, topic3, topic4, topic5);
    };
  } else {
    if (primary) {
      return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT1_PUB_HEADER_DEVICE, topic1, topic2, topic3, topic4, topic5);
    } else {
      return malloc_stringf("%s%s/%s/%s/%s/%s", MQTT2_PUB_HEADER_DEVICE, topic1, topic2, topic3, topic4, topic5);
    };
  };
}

char * mqttGetTopicDevice(const bool primary, const bool local, const char *topic1, const char *topic2, const char *topic3)
{
  if (topic3) {
    return mqttGetTopicDevice3(primary, local, topic1, topic2, topic3);
  }
  else if (topic2) {
    return mqttGetTopicDevice2(primary, local, topic1, topic2);
  }
  else if (topic1) {
    return mqttGetTopicDevice1(primary, local, topic1);
  }
  else {
    return nullptr;
  }
}

