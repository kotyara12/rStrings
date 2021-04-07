#include "rStrings.h"
#include "rLog.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static const char * tagHEAP = "OUT OF MEMORY";

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

