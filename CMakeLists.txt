cmake_minimum_required(VERSION 3.20)

# Project setup
set(PROJECT_NAME "Prism")
project(${PROJECT_NAME})

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(APPLE)
    # Find required packages on macOS
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(Stb REQUIRED)
    if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
        find_package(GTest CONFIG REQUIRED)
    endif()

    # Add M1 Mac support
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64")
    endif()

    # Add macOS compile definitions
    add_compile_definitions(GL_SILENCE_DEPRECATION)
    add_compile_definitions(GLFW_INCLUDE_GLCOREARB)
else()
    # Find required packages on non-macOS platforms
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(Stb REQUIRED)
    if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
        find_package(GTest CONFIG REQUIRED)
    endif()
endif()

# Define source and header files
file(GLOB_RECURSE LIB_SOURCES
    "src/*.cpp"
    "src/ECS/*.cpp"
    "src/ECS/Components/*.cpp"
    "src/ECS/Systems/*.cpp"
)
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Components/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ECS/Systems/*.h"
)

# Create library target
add_library(${PROJECT_NAME}_lib STATIC ${LIB_SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

# Link platform-specific libraries to library target
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

# Add stb include directories to the library target
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${Stb_INCLUDE_DIR})

# Create executable target
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Add stb include directories to the executable target
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

# Add tests in Debug and RelWithDebInfo modes
if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
    enable_testing()
    add_subdirectory(tests)
endif()

# Copy assets directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
