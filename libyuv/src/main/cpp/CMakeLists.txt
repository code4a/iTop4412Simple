# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

project(YUV C CXX) # "C" is required even for C++ projects
# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.4.1)

# 设置编译产出的so路径，在main/jniLibs/abi下
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

#添加日志打印出来
message(CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR}")

set(LY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(LY_TEST_DIR ${CMAKE_SOURCE_DIR}/unit_test)
set(LY_UTIL_DIR ${CMAKE_SOURCE_DIR}/util)

file(GLOB_RECURSE LY_SOURCE_FILES ${LY_SOURCE_DIR}/*.cc)
list(SORT LY_SOURCE_FILES)

file(GLOB_RECURSE LY_TEST_SOURCES ${LY_TEST_DIR}/*.cc)
list(SORT LY_TEST_SOURCES)

file(GLOB_RECURSE LY_UTIL_SOURCES ${LY_UTIL_DIR}/*.cc)
list(SORT LY_UTIL_SOURCES)

#设置头文件搜索路径（和此txt同个路径的头文件无需设置），可选
#导入include相关文件
include_directories(include)

# this creates the static library (.a)
#add_library				( libyuva STATIC ${LY_SOURCE_FILES} )
# this creates the shared library (.so)
add_library(yuv SHARED ${LY_SOURCE_FILES})

# 添加静态库，关键词为static，
# ADD_LIBRARY (hello STATIC ${LIBHELLO_SRC})
# 仍然用hello作为target名时，是不能成功创建所需的静态库的，
# 因为hello作为一个target是不能重名的， 故把上面的hello修改为hello_static
# 同理，你不需要写全libhello_static.a
# 只需要填写hello即可，cmake系统会自动为你生成 libhello_static.X
# ADD_LIBRARY (hello_static STATIC ${LIBHELLO_SRC})

# 按照一般的习惯，静态库名字跟动态库名字应该是一致的，只是扩展名不同；
# 即：静态库名为 libhello.a； 动态库名为libhello.so ；
# 所以，希望 "hello_static" 在输出时，不是"hello_static"，而是以"hello"的名字显示，故设置如下：
# SET_TARGET_PROPERTIES (hello_static PROPERTIES OUTPUT_NAME "hello")
# GET_TARGET_PROPERTY (OUTPUT_VALUE hello_static OUTPUT_NAME)
# MESSAGE (STATUS "This is the hello_static OUTPUT_NAME: " ${OUTPUT_VALUE})

# this creates the conversion tool
# add_executable			( yuvconvert ${LY_UTIL_DIR}/yuvconvert.cc )
# target_link_libraries	( yuvconvert libyuva )

#添加子目录，将自动找到子目录中的CMakeLists.txt
#ADD_SUBDIRECTORY(${LY_SOURCE_DIR})

# 我们需要将libhello.a, libhello.so.x以及hello.h安装到系统目录，才能真正让其他人开发使用，
# 在本例中我们将hello的共享库安装到<prefix>/lib目录；
# 将hello.h安装<prefix>/include/hello目录。
# install(TARGETS yuv LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
#INSTALL (FILES hello.h DESTINATION include/hello)