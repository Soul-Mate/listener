# Cmake 最低版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# 项目名称
PROJECT(listener C)

# 添加DEBUG宏定义
ADD_DEFINITIONS(-DDEBUG -DRUN -D_GNU_SOURCE -DLINUX)

ADD_SUBDIRECTORY(src)

#ADD_SUBDIRECTORY(tests)
