LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := png_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libpng.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_image/external/libpng-1.6.37
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogg_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libogg.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_mixer/external/libogg-1.3.2
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vorbisidec_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libvorbisidec.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_mixer/external/libvorbisidec-1.2.1
LOCAL_STATIC_LIBRARIES := ogg_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cpufeatures_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libcpufeatures.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := hidapi_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libhidapi.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_main
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2main.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2/include
LOCAL_STATIC_LIBRARIES := cpufeatures_static hidapi_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_image_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2_image.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_image
LOCAL_STATIC_LIBRARIES := png_static SDL2_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_mixer_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libSDL2_mixer.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/SDL2_mixer
LOCAL_STATIC_LIBRARIES := ogg_static vorbisidec_static SDL2_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RakNet_static
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libRakNet.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/RakNet/..
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Cheese-Engine
LOCAL_SRC_FILES := $(LOCAL_PATH)/../$(TARGET_ARCH_ABI)/libCheese-Engine.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/cheese-engine
LOCAL_STATIC_LIBRARIES := png_static ogg_static vorbisidec_static cpufeatures_static hidapi_static SDL2_main SDL2_static SDL2_image_static SDL2_mixer_static RakNet_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/boost

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../../../../*.cpp)

LOCAL_STATIC_LIBRARIES := png_static ogg_static vorbisidec_static cpufeatures_static hidapi_static SDL2_main SDL2_static SDL2_image_static SDL2_mixer_static RakNet_static Cheese-Engine

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -lz -landroid

include $(BUILD_SHARED_LIBRARY)
