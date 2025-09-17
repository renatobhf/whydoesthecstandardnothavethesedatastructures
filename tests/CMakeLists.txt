cmake_minimum_required(VERSION 3.5)

# Path to Unity submodule
set(UNITY_DIR "${CMAKE_SOURCE_DIR}/third_party/unity")

if (NOT EXISTS "${UNITY_DIR}/src/unity.c")
    message(FATAL_ERROR "Unity not found at ${UNITY_DIR}. Add it as a submodule:\n"
        "  git submodule add https://github.com/ThrowTheSwitch/Unity.git third_party/unity\n"
        "  git submodule update --init --recursive")
endif()

# Test executable
set(TEST_SRC
    test_dyn_array.c
    ${UNITY_DIR}/src/unity.c
)

add_executable(dyn_array_tests ${TEST_SRC})

target_include_directories(dyn_array_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/data-structures/
    ${UNITY_DIR}/src
)

target_compile_definitions(dyn_array_tests PRIVATE UNITY_INCLUDE_DOUBLE_SUPPORT)

# Add test to CTest
add_test(NAME dyn_array_tests COMMAND dyn_array_tests)
