#project name
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) #cmake最低版本需求，不加入此行会受到警告信息
PROJECT(HELLO) #项目名称　　

INCLUDE_DIRECTORIES(system ${PROJECT_SOURCE_DIR}/../dep/gflags/include/)
INCLUDE_DIRECTORIES(system ${PROJECT_SOURCE_DIR}/../dep/SimpleIni/)

# lib directories
LINK_DIRECTORIES(

  #项目引用第三方库
  ${PROJECT_SOURCE_DIR}/../dep/gflags/lib/
  
  #系统目录
  /usr/local/lib
  /use/lib
)

# 源文件搜索
FILE(GLOB_RECURSE SourceFiles 
        "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.cc") 
        
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -pthread")
ADD_EXECUTABLE(hello ${SourceFiles}) 


# 链接选项
#SET(CMAKE_EXE_LINKER_FLAGS "-rdynamic -Wl,-Bstatic -Wl,-Bdynamic -lstdc++ -lpthread -ldl -lz -lrt")
# 设置链接库 
TARGET_LINK_LIBRARIES(hello libgflags.a libgflags_nothreads.a)

#add_custom_command(
#     OUTPUT o1
#     DEPENDS f2 f4*
#     COMMAND cat f2 > o1
#     COMMENT "build OUTPUT "
#           )
#
#add_custom_target(t1 ALL DEPENDS o1)
#ADD_DEPENDENCIES(hello t1)