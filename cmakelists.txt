cmake_minimum_required(VERSION 3.20)

# Setup vars
set(PROJECT_NAME "OpenGLRenderer")
project(${PROJECT_NAME})

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Platform specific settings
if(APPLE)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(GTest CONFIG REQUIRED)

    # Add M1 Mac support
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64")
    endif()

    # Add macOS compile definitions
    add_compile_definitions(GL_SILENCE_DEPRECATION)
    add_compile_definitions(GLFW_INCLUDE_GLCOREARB)
else()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(GTest CONFIG REQUIRED)
endif()

# Define source files
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Define header files
file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Create library target
add_library(${PROJECT_NAME}_lib ${LIB_SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

# Link platform-specific libraries to lib target
if(APPLE)
    target_link_libraries(${PROJECT_NAME}_lib PUBLIC
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        glfw
        glm::glm
        imgui::imgui
    )
else()
    target_link_libraries(${PROJECT_NAME}_lib PUBLIC
        OpenGL::GL
        glfw
        GLEW::GLEW
        glm::glm
        imgui::imgui
    )
endif()

# Create executable target
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Enable testing
enable_testing()
add_subdirectory(tests)

# Copy assets directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
