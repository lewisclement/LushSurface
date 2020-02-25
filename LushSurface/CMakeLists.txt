cmake_minimum_required (VERSION 3.1)
project (LushSurface)

set(SOURCES
    main.cpp
    geometry.cpp

    engine/game.cpp
    engine/chunk.cpp
    engine/entity.cpp
    engine/entityset.cpp
    engine/renderengine.cpp
    engine/shader.cpp
    engine/simplex.cpp
    engine/simplexnoise.cpp
    engine/view.cpp
    engine/world.cpp

    engine/entities/player.cpp
)

set(HEADERS
    pch.hpp
    settings.hpp

    geometry.hpp

    engine/game.hpp
    engine/chunk.hpp
    engine/entity.hpp
    engine/entityset.hpp
    engine/renderengine.hpp
    engine/shader.hpp
    engine/simplex.hpp
    engine/simplexnoise.hpp
    engine/view.hpp
    engine/world.hpp

    engine/entities/player.hpp
)


set(CMAKE_CXX_STANDARD 11)
if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(EXT_PROJECTS_DIR "./lib")

set(SDL2_DIR "SDL2")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_BUILD_TYPE Debug)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_BUILD_TYPE Release)
endif()


if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCH "x64")
else()
	set(ARCH "x86")
endif()


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")


if(MSVC)
	set(SDL2_PATH "${LIB_DIR}/MSVC/${SDL2_DIR}")
elseif(MINGW)
	Message(FATAL_ERROR "MinGW not supported right now!")
endif()
set(BULLET_ROOT "${PROJECT_SOURCE_DIR}/bullet3/")


find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED sdl2>=2.0.0)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)


add_executable(${PROJECT_NAME} "main.cpp")
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES} ${HEADERS} ${LIB_SOURCES})

set(REQUIRED_INCLUDES
        ${OPENGL_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${BULLET_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIR}
)
set(REQUIRED_LIBRARIES
        ${OPENGL_LIBRARY}
        ${SDL2_LIBRARY}
        ${BULLET_LIBRARIES}
        ${GLEW_LIBRARY}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${REQUIRED_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBRARIES})


if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif (MSVC)

foreach(FILE ${SOURCES} ${HEADERS} ${TEST_FILES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.hpp")
       set(GROUP "${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_definitions(-DOPENGLES_VERSION_MAJOR=3)
add_definitions(-DOPENGLES_VERSION_MINOR=3)

