
file(GLOB proj_files *.cpp)

add_library(ffmpeg_uvc_stream
        SHARED
        ${proj_files})

# 获取module根目录
set(YUV_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../libyuv)
message(YUV_ROOT_DIR " ${YUV_ROOT_DIR}")

add_library(yuv
        SHARED
        IMPORTED)

# 不依赖module，直接加载so文件，注意需要配置libyuv的camke文件指定so产出目录
#set_target_properties(yuv
#        PROPERTIES IMPORTED_LOCATION
#        ${YUV_ROOT_DIR}/src/main/jniLibs/${ANDROID_ABI}/libyuv.so
#        )
# 依赖module，加载自动生成的so文件
set_target_properties(yuv
        PROPERTIES IMPORTED_LOCATION
        ${YUV_ROOT_DIR}/build/intermediates/cmake/${CMAKE_BUILD_TYPE}/obj/${ANDROID_ABI}/libyuv.so
        )

# libyuv库头文件
include_directories(${YUV_ROOT_DIR}/src/main/cpp/include)

target_link_libraries(ffmpeg_uvc_stream
        # Links FFmpeg
        avfilter avcodec avdevice avformat avutil postproc swresample swscale
        # 链接 yuv
        yuv
        android
        log)