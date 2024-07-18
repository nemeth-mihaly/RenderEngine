cmake_minimum_required(VERSION 3.30.0-rc1)

project(Engine VERSION 1.0.0)

file(GLOB_RECURSE Sources "Source/*.c" "Source/*.cpp")

add_executable(Engine ${Sources})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

add_compile_options(-Wall -Wextra -Wpedantic -ggdb)

set(Msys2Libraries "C:/MSYS64/ucrt64/lib")

target_link_libraries(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Lib/libglfw3dll.a)
target_link_libraries(Engine PUBLIC ${Msys2Libraries}/libopengl32.a)

install(TARGETS Engine RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Bin)