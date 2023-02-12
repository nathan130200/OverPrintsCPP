file(GLOB APPCORE_SOURCES "*.cpp")
file(GLOB APPCORE_HEADERS "*.h")

add_library(AppCore STATIC
	${APPCORE_SOURCES}
	${APPCORE_HEADERS}
)

target_link_libraries(AppCore PUBLIC ImGui)
target_include_directories(AppCore PUBLIC ImGui)