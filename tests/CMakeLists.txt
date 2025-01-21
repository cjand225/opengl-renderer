file(GLOB_RECURSE TEST_SOURCES
    "src/*.cpp"
    "src/ecs/*.cpp"
    "src/ecs/Components/*.cpp"
    "src/ecs/Systems/*.cpp"
)

add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    ${PROJECT_NAME}_lib)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_command(TARGET unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)

include(GoogleTest)
gtest_discover_tests(unit_tests)
