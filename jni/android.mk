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

#必须位于Android.mk文件的最开始。用来定位源文件的位置，$(call my-dir)的作用就是返回当前目录的路径。
#$(call my-dir)只需要被调用1次
LOCAL_PATH := $(call my-dir)

#清除一些变量的值，但是LOCAL_PATH除外
include $(CLEAR_VARS)

#是用来指定当前待编译模块的名
LOCAL_MODULE    := bstruct

#是用来指定参与编译的源代码文件 换行连接符\.
bstruct_source		:=  ../source/BArray.cpp \
					../source/BStruct.cpp \
					../source/Stream.cpp

LOCAL_SRC_FILES := ../source/BArray.cpp ../source/BStruct.cpp ../source/Stream.cpp

#引用外部的C头文件，引用规则同make文件外部头文件
#LOCAL_CFLAGS := / -I/home/huoyu/include 

#引用外部库文件，引用规则同make文件外部库文件
#LOCAL_LDLIBS := -L$(HuoYuLib)/usr/lib -llog -lpcap -L/home/huoyu/lib


#将当前模块编译为共享库，前缀为lib，后缀为.so
#include $(BUILD_SHARED_LIBRARY)

#将当前模块编译为共享库，前缀为lib，后缀为.a，android只能调用so格式的本地库
include $(BUILD_STATIC_LIBRARY)

#调用所有子目录中的Android.mk文件
#include $(call all-subdir-makefiles)
