#继承上一层的CMakeLists.txt的变量，也可以在这里重新赋值
#C 的编译选项是 CMAKE_C_FLAGS
# 指定编译参数，可选
#SET(CMAKE_CXX_FLAGS "-Wno-error=format-security -Wno-error=pointer-sign")

# 添加rtmp推流
add_library(ffmpeg_rtmp SHARED ffmpeg_rtmp.cpp)

# 搜索指定的预构建库，并将路径存储为变量。由于默认情况下CMake在搜索路径中包括系统库，
# 因此您只需要指定要添加的公共NDK库的名称即可。在完成构建之前，CMake会验证该库是否存在。
# 日志 so
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# 连接到rtmp推流库
target_link_libraries( # Specifies the target library.
        ffmpeg_rtmp
        # Links FFmpeg
        avfilter avcodec avformat avutil postproc swresample swscale
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})