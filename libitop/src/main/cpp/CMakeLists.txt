# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_CXX_FLAGS "-Wno-error=format-security -Wno-error=pointer-sign")
# 设置编译产出的so路径，在main/jniLibs/abi下
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

#设置头文件搜索路径（和此txt同个路径的头文件无需设置），可选
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/common)

#指定用到的系统库或者NDK库或者第三方库的搜索路径，可选。
#LINK_DIRECTORIES(/usr/local/lib)

#添加子目录，将自动找到子目录中的CMakeLists.txt
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/motor)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/relay)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/rifd)
#ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/main/cpp/uvc)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/l9110s)
