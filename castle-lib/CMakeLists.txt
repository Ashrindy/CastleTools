project("castle-lib")

add_library(castle-lib STATIC 
    src/castle-lib/animation.cpp
    src/castle-lib/archive.cpp
    src/castle-lib/heightmaplevel.cpp
    src/castle-lib/model.cpp
    src/castle-lib/terrainlevelmap.cpp
)
option(CASTLELIB_RDA_MULTITHREADED_UNCOMPRESS "Use multi-threading for uncompressing archives (RDA)." ON)
option(CASTLELIB_RDA_MULTITHREADED_COMPRESS "Use multi-threading for compressing archives (RDA)." ON)

target_compile_features(castle-lib PUBLIC cxx_std_20)
target_include_directories(castle-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(ZLIB REQUIRED)
target_link_libraries(castle-lib PRIVATE ZLIB::ZLIB)
if(CASTLELIB_RDA_MULTITHREADED_UNCOMPRESS)
    target_compile_definitions(castle-lib PRIVATE CASTLELIB_RDA_MULTITHREADED_UNCOMPRESS)
endif()
if(CASTLELIB_RDA_MULTITHREADED_COMPRESS)
    target_compile_definitions(castle-lib PRIVATE CASTLELIB_RDA_MULTITHREADED_COMPRESS)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions(castle-lib PRIVATE NO_ICONV=1)
        message(FATAL_ERROR "No ICONV with this configuration")
    endif()
endif()
