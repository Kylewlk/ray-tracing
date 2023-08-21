cmake_minimum_required(VERSION 3.20)

project(ray-tracing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

add_subdirectory(libs)

function(setup_app app)
    target_link_libraries(${app} ray-tracing-libs)

    target_include_directories(${app} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_include_directories(${app} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ray-tracing-1)

    target_compile_options(${app} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    target_compile_definitions(${app} PRIVATE CURRENT_WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()

file(GLOB_RECURSE src_code LIST_DIRECTORIES false
        src/*.hpp src/*.h src/*.cpp src/*.c )

file(GLOB_RECURSE app1_code LIST_DIRECTORIES false
        ray-tracing-1/*.hpp ray-tracing-1/*.h
        ray-tracing-1/*.cpp ray-tracing-1/*.c )

add_executable(app1 ${src_code} ${app1_code})

setup_app(app1)

