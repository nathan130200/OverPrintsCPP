file(GLOB NODE_EDITOR_SOURCES "*.cpp")
file(GLOB NODE_EDITOR_HEADERS "*.h")

add_library(NodeEditor STATIC
	${NODE_EDITOR_SOURCES}
	${NODE_EDITOR_HEADERS}
)

target_include_directories(NodeEditor PUBLIC ImGui)
target_link_libraries(NodeEditor PUBLIC ImGui)