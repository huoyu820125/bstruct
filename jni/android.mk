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

#����λ��Android.mk�ļ����ʼ��������λԴ�ļ���λ�ã�$(call my-dir)�����þ��Ƿ��ص�ǰĿ¼��·����
#$(call my-dir)ֻ��Ҫ������1��
LOCAL_PATH := $(call my-dir)

#���һЩ������ֵ������LOCAL_PATH����
include $(CLEAR_VARS)

#������ָ����ǰ������ģ�����
LOCAL_MODULE    := bstruct

#������ָ����������Դ�����ļ� �������ӷ�\.
bstruct_source		:=  ../source/BArray.cpp \
					../source/BStruct.cpp \
					../source/Stream.cpp

LOCAL_SRC_FILES := ../source/BArray.cpp ../source/BStruct.cpp ../source/Stream.cpp

#�����ⲿ��Cͷ�ļ������ù���ͬmake�ļ��ⲿͷ�ļ�
#LOCAL_CFLAGS := / -I/home/huoyu/include 

#�����ⲿ���ļ������ù���ͬmake�ļ��ⲿ���ļ�
#LOCAL_LDLIBS := -L$(HuoYuLib)/usr/lib -llog -lpcap -L/home/huoyu/lib


#����ǰģ�����Ϊ����⣬ǰ׺Ϊlib����׺Ϊ.so
#include $(BUILD_SHARED_LIBRARY)

#����ǰģ�����Ϊ����⣬ǰ׺Ϊlib����׺Ϊ.a��androidֻ�ܵ���so��ʽ�ı��ؿ�
include $(BUILD_STATIC_LIBRARY)

#����������Ŀ¼�е�Android.mk�ļ�
#include $(call all-subdir-makefiles)
