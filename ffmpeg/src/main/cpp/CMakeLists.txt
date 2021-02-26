# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#添加日志打印出来
message(CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR}")

#设置头文件搜索路径（和此txt同个路径的头文件无需设置），可选
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/common)
#导入include相关文件
include_directories(include)
# 加载整个文件夹下所有的so
set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
#赋值CMAKE_CXX_FLAGS 实际目录libs/armeabi-v7a
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${JNI_LIBS_DIR}")

# 上面的代码等同于下面的代码，区别在于，上面是加载整个文件夹下所有的so,下面必须单个添加
# set(JNI_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../main/jniLibs/${ANDROID_ABI})
##加载预编译好的so库
#add_library(avcodec SHARED IMPORTED)
#set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libavcodec.so LINKER_LANGUAGE CXX)

#指定用到的系统库或者NDK库或者第三方库的搜索路径，可选。
#LINK_DIRECTORIES(/usr/local/lib)

#添加子目录，将自动找到子目录中的CMakeLists.txt
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/player)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/rtmp)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/stream)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/uvc)
