cmake_minimum_required(VERSION 4.1)
project(hot_entities)

set(CMAKE_CXX_STANDARD 23)

add_library(hot_entities INTERFACE
        hot_entities/EntityManager.hpp
        hot_entities/IEntity.hpp
)

target_include_directories(hot_entities INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)