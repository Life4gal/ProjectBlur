project(PB-Infra)

add_library(
    ${PROJECT_NAME}
    STATIC
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    
    ${PB_COMPILE_FLAGS}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    
    PB_MAJOR_VERSION=${PB_MAJOR_VERSION}
    PB_MINOR_VERSION=${PB_MINOR_VERSION}
    PB_PATCH_VERSION=${PB_PATCH_VERSION}
    PB_VERSION="${PB_VERSION}"
    
   PB_PROJECT_NAME="${PROJECT_NAME}"
   PB_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
   PB_GIT_COMMIT_INFO="${PB_GIT_COMMIT_INFO}"
    
    ${PB_PLATFORM_NAME}
    
    # msvc
    $<$<CXX_COMPILER_ID:MSVC>:PB_COMPILER_MSVC>
    # g++
    $<$<CXX_COMPILER_ID:GNU>:PB_COMPILER_GNU>
    # clang-cl
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<STREQUAL:"${CMAKE_CXX_SIMULATE_ID}","MSVC">>:PB_COMPILER_CLANG_CL>
    # clang
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<STREQUAL:"${CMAKE_CXX_SIMULATE_ID}","MSVC">>>:PB_COMPILER_CLANG>
    # apple clang
    $<$<CXX_COMPILER_ID:AppleClang>:PB_COMPILER_APPLE_CLANG>
)

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_23
)

set(
    PB_INFRA_PUBLIC_FILES 

    # =========================
    # UTILITY
    # =========================

    ${CMAKE_CURRENT_SOURCE_DIR}/include/pb/utility/guard.hpp
)

set(
    PB_INFRA_PRIVATE_FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp
)

set_source_files_properties(
    ${PB_INFRA_PUBLIC_FILES}
    ${PB_INFRA_PRIVATE_FILES}
    PROPERTIES
    LANGUAGE CXX
)

target_sources(
    ${PROJECT_NAME}
    PUBLIC
    FILE_SET public_header_files
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES
    
    ${PB_INFRA_PUBLIC_FILES}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${PB_INFRA_PRIVATE_FILES}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    VERSION ${PB_VERSION}
    SOVERSION ${PB_MAJOR_VERSION}
    PUBLIC_HEADER "${PB_HEADER}"
    DEBUG_POSTFIX "${d}"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)