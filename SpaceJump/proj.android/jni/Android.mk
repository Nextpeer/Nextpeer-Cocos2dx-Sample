LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -DCC_ENABLE_BOX2D_INTEGRATION=1

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp

CPP_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
CPP_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Nextpeer/Nextpeer-Cocos2D-X/Classes/*.cpp)
CPP_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Nextpeer/Nextpeer-Cocos2D-X/Android/*.cpp)
CPP_FILES += $(wildcard $(LOCAL_PATH)/../../Classes/Nextpeer/Nextpeer-Cocos2D-X/Classes/mt_rand/*.cpp)
LOCAL_SRC_FILES += $(CPP_FILES:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := \
					$(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes/Nextpeer/Nextpeer-Cocos2D-X/Classes \
                    $(LOCAL_PATH)/../../Classes/Nextpeer/Nextpeer-Cocos2D-X/Android \
                    $(LOCAL_PATH)/../../Classes/Nextpeer/Nextpeer-Cocos2D-X/Classes/mt_rand

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
