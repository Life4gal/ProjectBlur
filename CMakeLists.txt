cmake_minimum_required(VERSION 3.25)

set(PB_MAJOR_VERSION 0 CACHE STRING "ProjectBlur major version")
set(PB_MINOR_VERSION 1 CACHE STRING "ProjectBlur minor version")
set(PB_PATCH_VERSION 0 CACHE STRING "ProjectBlur patch version")
set(PB_VERSION "${PB_MAJOR_VERSION}.${PB_MINOR_VERSION}.${PB_PATCH_VERSION}" CACHE STRING "ProjectBlur version")

project(
		ProjectBlur
		VERSION ${PB_VERSION}
		DESCRIPTION "ProjectBlur"
		HOMEPAGE_URL "https://github.com/Life4gal/ProjectBlur"
		LANGUAGES CXX
)

# ===================================================================================================
# PLATFORM

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(PB_PLATFORM_WINDOWS ON)
	set(PB_PLATFORM_NAME PB_PLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(PB_PLATFORM_LINUX ON)
	set(PB_PLATFORM_NAME PB_PLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(PB_PLATFORM_DARWIN ON)
	set(PB_PLATFORM_NAME PB_PLATFORM_DARWIN)
else ()
	message(FATAL_ERROR "[ProjectBlur] Unknown Platform: ${CMAKE_SYSTEM_NAME}")
endif (CMAKE_SYSTEM_NAME STREQUAL "Windows")

# ===================================================================================================
# COMPILER

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(PB_COMPILER_MSVC ON)
	set(PB_COMPILER_NAME "MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
		set(PB_COMPILER_CLANG_CL ON)
		set(PB_COMPILER_NAME "ClangCL")
	else ()
		set(PB_COMPILER_CLANG ON)
		set(PB_COMPILER_NAME "Clang")
	endif (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(PB_COMPILER_GNU ON)
	set(PB_COMPILER_NAME "GNU")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(PB_COMPILER_CLANG_APPLE ON)
	set(PB_COMPILER_NAME "AppleClang")
else ()
	message(FATAL_ERROR "[ProjectBlur] Unknown compiler: ${CMAKE_CXX_COMPILER}")
endif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# ===================================================================================================
# COMPILE FLAGS

if (PB_COMPILER_MSVC)
	set(PB_COMPILE_FLAGS "/D_CRT_SECURE_NO_WARNINGS;/DNOMINMAX;/utf-8;/W3;/WX;/Zc:preprocessor")
elseif (PB_COMPILER_CLANG_CL)
	set(PB_COMPILE_FLAGS "/D_CRT_SECURE_NO_WARNINGS;/DNOMINMAX;/utf-8;/W3;/WX")
elseif (PB_COMPILER_CLANG)
	set(PB_COMPILE_FLAGS "-Wall;-Wextra;-Wpedantic;-Werror")
elseif (PB_COMPILER_GNU)
	set(PB_COMPILE_FLAGS "-Wall;-Wextra;-Wpedantic;-Werror")
elseif (PB_COMPILER_CLANG_APPLE)
	set(PB_COMPILE_FLAGS "-Wall;-Wextra;-Wpedantic;-Werror")
endif (PB_COMPILER_MSVC)

# ===================================================================================================
# GIT

find_package(Git QUIET)
if(NOT GIT_FOUND)
    message(WARNING "Git not found - version information will not be available")

	set(PB_GIT_COMMIT_HASH "unknown" CACHE STRING "ProjectBlur git commit hash")
	set(PB_GIT_COMMIT_DATE "unknown" CACHE STRING "ProjectBlur git commit date")
	set(PB_GIT_DIRTY_FLAG "unknown" CACHE STRING "ProjectBlur git dirty")
	set(PB_GIT_COMMIT_INFO "unknown" CACHE STRING "ProjectBlur git commit info")
else()
	# 获取简短commit hash (默认7位)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PB_GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        RESULT_VARIABLE PB_GIT_REV_PARSE_RESULT
    )

    # 检查是否有未提交更改
    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --quiet --exit-code
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE PB_GIT_DIRTY
        ERROR_QUIET
    )

    # 获取提交时间
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd --date=iso
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PB_GIT_COMMIT_DATE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(PB_GIT_REV_PARSE_RESULT EQUAL "0")
        set(PB_GIT_COMMIT_HASH "${PB_GIT_COMMIT_HASH}" CACHE STRING "ProjectBlur git commit hash")
		set(PB_GIT_COMMIT_DATE "${PB_GIT_COMMIT_DATE}" CACHE STRING "ProjectBlur git commit date")

        if(PB_GIT_DIRTY EQUAL "0")
            set(PB_GIT_DIRTY_FLAG "" CACHE STRING "ProjectBlur git dirty")
        else()
            set(PB_GIT_DIRTY_FLAG "-dirty" CACHE STRING "ProjectBlur git dirty")
        endif(PB_GIT_DIRTY EQUAL "0")

        set(PB_GIT_COMMIT_INFO "${PB_GIT_COMMIT_HASH}${PB_GIT_DIRTY_FLAG} (${PB_GIT_COMMIT_DATE})" CACHE STRING "ProjectBlur git commit info")
    endif(PB_GIT_REV_PARSE_RESULT EQUAL "0")
endif(NOT GIT_FOUND)

# ===================================================================================================
# OUTPUT INFO

message(STATUS "[ProjectBlur] CMAKE VERSION: ${CMAKE_VERSION}. Platform: ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}.")
message(STATUS "[ProjectBlur] PROJECT FULL NAME: ${PROJECT_NAME}-${PB_VERSION} (${CMAKE_BUILD_TYPE}) ${PB_GIT_COMMIT_INFO}")

# ===================================================================================================
# DEPENDENCIES

# SDL
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
#find_package(SDL3_mixer CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

# GLM
find_package(GLM REQUIRED)

# IMGUI
find_package(imgui CONFIG REQUIRED)

# EnTT
find_package(EnTT CONFIG REQUIRED)

# nlohmann::json
find_package(nlohmann_json CONFIG REQUIRED)

# spdlog
find_package(spdlog CONFIG REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${CMAKE_SOURCE_DIR}/infra)
add_subdirectory(${CMAKE_SOURCE_DIR}/core)
