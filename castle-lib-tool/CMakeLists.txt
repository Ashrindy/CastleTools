project("castle-lib-tool" VERSION 0.0.1)
add_executable(castle-lib-tool 
    "src/castle-lib-tool.cpp" 
    "src/archive-handler.cpp" 
    "src/commands.cpp" 
    #"src/model-handler.cpp"
)
target_include_directories(castle-lib-tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(castle-lib-tool castle-lib assimp)
target_compile_definitions(castle-lib-tool PRIVATE 
  CASTLELIBTOOL_VERSION=${PROJECT_VERSION}
  CASTLELIBTOOL_VERSION_S="${PROJECT_VERSION}"
)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions(castle-lib-tool PRIVATE NO_ICONV=1)
    endif()
endif()
