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
LOCAL_PATH	:= $(call my-dir)/..

include $(CLEAR_VARS)

FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.cpp) $(LOCAL_PATH)/src/GLES/Platform.cpp $(wildcard $(LOCAL_PATH)/src/Android/*.cpp)

ifdef DEBUG
CONFIG_DIR	:= Debug
LOCAL_CFLAGS    := -Werror -Wno-psabi -O0 -ggdb -D_DEBUG
LOCAL_CXXFLAGS    := -Werror -Wno-psabi -O0 -ggdb -D_DEBUG -fexceptions
LOCAL_LINK_FLAGS	:= -ggdb
else
CONFIG_DIR	:= Release
LOCAL_CFLAGS	:= -Werror -Wno-psabi -O2 -DNDEBUG
LOCAL_CXXFLAGS	:= -Werror -Wno-psabi -O2 -DNDEBUG -fexceptions
endif

LOCAL_MODULE    := EffectsManager
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/include/Platform/GLES/Android $(LOCAL_PATH)/../jsoncpp/include $(LOCAL_PATH)/../rapidxml/include $(LOCAL_PATH)/../SOIL/include
LOCAL_LDFLAGS	:= -L../jsoncpp/libs/Android/$(CONFIG_DIR)/local/$(TARGET_ARCH_ABI) -L../SOIL/libs/Android/$(CONFIG_DIR)/local/$(TARGET_ARCH_ABI) $(LOCAL_LINK_FLAGS)
LOCAL_LDLIBS	+= -lGLESv2 -ljsoncpp -lSOIL -landroid -llog -lGLESv1_CM -lEGL

include $(BUILD_SHARED_LIBRARY)

