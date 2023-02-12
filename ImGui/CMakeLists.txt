file(GLOB IMGUI_SOURCES "*.cpp" "*.c")
file(GLOB IMGUI_HEADERS "*.h")

add_library(ImGui STATIC 
	${IMGUI_SOURCES}
	${IMGUI_HEADERS}
)

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})