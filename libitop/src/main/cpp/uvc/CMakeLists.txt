#继承上一层的CMakeLists.txt的变量，也可以在这里重新赋值
#C 的编译选项是 CMAKE_C_FLAGS
# 指定编译参数，可选
#SET(CMAKE_CXX_FLAGS "-Wno-error=format-security -Wno-error=pointer-sign")

#生成动态库名称、类型、资源文件
add_library(itop_uvc SHARED itop_uvc.c)

# 获取module根目录
set(FF_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../ffmpeg)
message(FF_ROOT_DIR " ${FF_ROOT_DIR}")

#导入include相关文件
include_directories(${FF_ROOT_DIR}/src/main/cpp/include)
# 加载整个文件夹下所有的so
set(JNI_LIBS_DIR ${FF_ROOT_DIR}/src/main/jniLibs/${ANDROID_ABI})
#赋值CMAKE_CXX_FLAGS 实际目录libs/armeabi-v7a
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${JNI_LIBS_DIR}")

#依赖库
find_library(log-lib log)
#依赖的目标库
target_link_libraries(itop_uvc
        # Links FFmpeg
        avfilter avcodec avformat avutil postproc swresample swscale
        ${log-lib})