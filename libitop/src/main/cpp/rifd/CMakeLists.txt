#继承上一层的CMakeLists.txt的变量，也可以在这里重新赋值
#C 的编译选项是 CMAKE_C_FLAGS
# 指定编译参数，可选
#SET(CMAKE_CXX_FLAGS "-Wno-error=format-security -Wno-error=pointer-sign")

#生成动态库名称、类型、资源文件
add_library(itop_rfid SHARED itop_rfid.c)
#依赖库
find_library(log-lib log)
#依赖的目标库
target_link_libraries(itop_rfid ${log-lib})