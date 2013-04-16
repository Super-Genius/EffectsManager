
ifdef NDK_DEBUG
CONFIG_DIR	:= Debug
APP_OPTIM 	:= debug
DEBUG		:= 1
else
CONFIG_DIR	:= Release
APP_OPTIM	:= release
endif

APP_STL := gnustl_static
APP_MODULES := EffectsManager
NDK_APP_OUT := $(call my-dir)/../libs/Android/$(CONFIG_DIR)
APP_ABI := all
APP_PLATFORM := android-9
