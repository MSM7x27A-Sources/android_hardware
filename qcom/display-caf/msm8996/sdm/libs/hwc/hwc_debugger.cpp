/*
* Copyright (c) 2014 - 2015, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of The Linux Foundation nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <utils/constants.h>
#include <cutils/properties.h>

#include "hwc_debugger.h"

namespace sdm {

HWCDebugHandler HWCDebugHandler::debug_handler_;
uint32_t HWCDebugHandler::debug_flags_ = 0x1;
uint32_t HWCDebugHandler::verbose_level_ = 0x0;

void HWCDebugHandler::DebugAll(bool enable, int verbose_level) {
  if (enable) {
    debug_flags_ = 0xFFFFFFFF;
    verbose_level_ = verbose_level;
  } else {
    debug_flags_ = 0x1;   // kTagNone should always be printed.
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::DebugResources(bool enable, int verbose_level) {
  if (enable) {
    SET_BIT(debug_flags_, kTagResources);
    verbose_level_ = verbose_level;
  } else {
    CLEAR_BIT(debug_flags_, kTagResources);
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::DebugStrategy(bool enable, int verbose_level) {
  if (enable) {
    SET_BIT(debug_flags_, kTagStrategy);
    verbose_level_ = verbose_level;
  } else {
    CLEAR_BIT(debug_flags_, kTagStrategy);
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::DebugCompManager(bool enable, int verbose_level) {
  if (enable) {
    SET_BIT(debug_flags_, kTagCompManager);
    verbose_level_ = verbose_level;
  } else {
    CLEAR_BIT(debug_flags_, kTagCompManager);
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::DebugDriverConfig(bool enable, int verbose_level) {
  if (enable) {
    SET_BIT(debug_flags_, kTagDriverConfig);
    verbose_level_ = verbose_level;
  } else {
    CLEAR_BIT(debug_flags_, kTagDriverConfig);
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::DebugRotator(bool enable, int verbose_level) {
  if (enable) {
    SET_BIT(debug_flags_, kTagRotator);
    verbose_level_ = verbose_level;
  } else {
    CLEAR_BIT(debug_flags_, kTagRotator);
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::DebugQdcm(bool enable, int verbose_level) {
  if (enable) {
    SET_BIT(debug_flags_, kTagQDCM);
    verbose_level_ = verbose_level;
  } else {
    CLEAR_BIT(debug_flags_, kTagQDCM);
    verbose_level_ = 0;
  }
}

void HWCDebugHandler::Error(DebugTag /*tag*/, const char *format, ...) {
  va_list list;
  va_start(list, format);
  __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, list);
}

void HWCDebugHandler::Warning(DebugTag /*tag*/, const char *format, ...) {
  va_list list;
  va_start(list, format);
  __android_log_vprint(ANDROID_LOG_WARN, LOG_TAG, format, list);
}

void HWCDebugHandler::Info(DebugTag tag, const char *format, ...) {
  if (IS_BIT_SET(debug_flags_, tag)) {
    va_list list;
    va_start(list, format);
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, list);
  }
}

void HWCDebugHandler::Debug(DebugTag tag, const char *format, ...) {
  if (IS_BIT_SET(debug_flags_, tag)) {
    va_list list;
    va_start(list, format);
    __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, format, list);
  }
}

void HWCDebugHandler::Verbose(DebugTag tag, const char *format, ...) {
  if (IS_BIT_SET(debug_flags_, tag)  && verbose_level_) {
    va_list list;
    va_start(list, format);
    __android_log_vprint(ANDROID_LOG_VERBOSE, LOG_TAG, format, list);
  }
}

void HWCDebugHandler::BeginTrace(const char *class_name, const char *function_name,
                                 const char *custom_string) {
  char name[PATH_MAX] = {0};
  snprintf(name, sizeof(name), "%s::%s::%s", class_name, function_name, custom_string);
  atrace_begin(ATRACE_TAG, name);
}

void HWCDebugHandler::EndTrace() {
  atrace_end(ATRACE_TAG);
}

int  HWCDebugHandler::GetIdleTimeoutMs() {
  int value = IDLE_TIMEOUT_DEFAULT_MS;
  debug_handler_.GetProperty("sdm.idle_time", &value);

  return value;
}

DisplayError HWCDebugHandler::GetProperty(const char *property_name, int *value) {
  char property[PROPERTY_VALUE_MAX];

  if (property_get(property_name, property, NULL) > 0) {
    *value = atoi(property);
    return kErrorNone;
  }

  return kErrorNotSupported;
}

DisplayError HWCDebugHandler::GetProperty(const char *property_name, char *value) {
  if (property_get(property_name, value, NULL) > 0) {
    return kErrorNone;
  }

  return kErrorNotSupported;
}

DisplayError HWCDebugHandler::SetProperty(const char *property_name, const char *value) {
  if (property_set(property_name, value) == 0) {
    return kErrorNone;
  }

  return kErrorNotSupported;
}

}  // namespace sdm

