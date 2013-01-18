 # Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
MY2_PATH := $(LOCAL_PATH)

include $(LOCAL_PATH)/../../../Engine/IDEProjects/Android.mk
include $(CLEAR_VARS)
LOCAL_PATH := $(MY2_PATH)/../Code

LOCAL_MODULE := sgLib
LOCAL_CFLAGS := -Werror

# Header search paths
LOCAL_CFLAGS += -I$(MY_PATH)/Common
LOCAL_CFLAGS += -I$(MY_PATH)/Graphics
LOCAL_CFLAGS += -I$(MY_PATH)/Input
LOCAL_CFLAGS += -I$(MY_PATH)/Audio
LOCAL_CFLAGS += -I$(MY_PATH)/Physics
LOCAL_CFLAGS += -I$(MY_PATH)/Combiner
LOCAL_CFLAGS += -I$(MY_PATH)/Templates
LOCAL_CFLAGS += -I$(MY_PATH)/OSBindings/Android

LOCAL_CFLAGS += -I$(LOCAL_PATH)/Main
LOCAL_CFLAGS += -I$(LOCAL_PATH)/Objects

# Defines
LOCAL_CFLAGS += -D__ANDROID__


# Source
LOCAL_SRC_FILES := Main/main.cpp
LOCAL_SRC_FILES += Main/Events.cpp
LOCAL_SRC_FILES += Objects/Car.cpp


LOCAL_LDLIBS := -llog -lz -lGLESv2

LOCAL_STATIC_LIBRARIES := libisdge libzip libpng

include $(BUILD_SHARED_LIBRARY)
