project(PB-Core)

add_executable(
    ${PROJECT_NAME}

    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
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

target_link_libraries(
   ${PROJECT_NAME} 
   PRIVATE 
   
   PB-Infra

   SDL3::SDL3
   SDL3_image::SDL3_image
   #SDL3_mixer::SDL3_mixer
   SDL3_ttf::SDL3_ttf
   imgui::imgui
   EnTT::EnTT
   nlohmann_json::nlohmann_json
   spdlog::spdlog
)

set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)