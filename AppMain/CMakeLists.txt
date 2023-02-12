file(GLOB APPMAIN_SOURCES "*.cpp")
file(GLOB APPMAIN_HEADERS "*.h")

add_executable(AppMain
	${APPMAIN_SOURCES}
	${APPMAIN_HEADERS}
)

target_link_libraries(AppMain 
	PUBLIC ImGui
	PUBLIC AppCore
	PUBLIC NodeEditor
)

set_target_properties(AppMain PROPERTIES WIN32_EXECUTABLE TRUE)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(AppMain PUBLIC "/ZI")
  target_link_options(AppMain PUBLIC "/SAFESEH:NO")
endif()