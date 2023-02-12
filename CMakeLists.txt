cmake_minimum_required(VERSION 3.20)

project("OverPrints")

add_definitions(
   /D_CRT_SECURE_NO_WARNINGS
   /D_SCL_SECURE_NO_WARNINGS
   /DNOMINMAX
   /DNT
   /DWIN32_LEAN_AND_MEAN
)

remove_definitions(-D_MBCS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("ImGui")
add_subdirectory("NodeEditor")
add_subdirectory("AppCore")
add_subdirectory("AppMain")