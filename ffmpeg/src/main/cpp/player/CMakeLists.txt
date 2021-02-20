#继承上一层的CMakeLists.txt的变量，也可以在这里重新赋值
#C 的编译选项是 CMAKE_C_FLAGS
# 指定编译参数，可选
#SET(CMAKE_CXX_FLAGS "-Wno-error=format-security -Wno-error=pointer-sign")

#project("ffmpeg")
#
#add_library(avcodec
#        SHARED
#        IMPORTED)
#set_target_properties(avcodec
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libavcodec.so)
#
#add_library(avformat
#        SHARED
#        IMPORTED)
#set_target_properties(avformat
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libavformat.so)
#
#add_library(avutil
#        SHARED
#        IMPORTED)
#set_target_properties(avutil
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libavutil.so)
#
#add_library(postproc
#        SHARED
#        IMPORTED)
#set_target_properties(postproc
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libpostproc.so)
#
#add_library(swresample
#        SHARED
#        IMPORTED)
#set_target_properties(swresample
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libswresample.so)
#
#add_library(swscale
#        SHARED
#        IMPORTED)
#set_target_properties(swscale
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libswscale.so)
#
#add_library(avfilter
#        SHARED
#        IMPORTED)
#set_target_properties(avfilter
#        PROPERTIES IMPORTED_LOCATION
#        ${DIR}/armeabi-v7a/libavfilter.so)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
# 如果我们要将当前目录下所有的*.cpp文件都一起编译的话，可以使
#file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#add_library(native-lib SHARED ${SOURCE} )
# 设置要编译的cpp文件
# 添加播放器
add_library(
        # Sets the name of the library.
        ffmpeg_player
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        ffmpeg_player.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
# 日志 so
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
# 添加播放器链接库
target_link_libraries( # Specifies the target library.
        ffmpeg_player
        # 链接OpenSLES库，功能非常强大的音频框架
        #        OpenSLES
        # Links FFmpeg
        avfilter avcodec avformat avutil postproc swresample swscale
        # Links Android ANativeWindow 需要导入android库，不然编译失败
        android
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})