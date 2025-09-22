cmake_minimum_required(VERSION 3.5)

# Path to Unity submodule
set(UNITY_DIR "${CMAKE_SOURCE_DIR}/third_party/unity")

if (NOT EXISTS "${UNITY_DIR}/src/unity.c")
    message(FATAL_ERROR "Unity not found at ${UNITY_DIR}. Add it as a submodule:\n"
        "  git submodule add https://github.com/ThrowTheSwitch/Unity.git third_party/unity\n"
        "  git submodule update --init --recursive")
endif()

########################################
# Dynamic Array Tests
########################################
set(DYN_ARRAY_TEST_SRC
    test_dyn_array.c
    ${UNITY_DIR}/src/unity.c
)

add_executable(dyn_array_tests ${DYN_ARRAY_TEST_SRC})

target_include_directories(dyn_array_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/data-structures/
    ${UNITY_DIR}/src
)

target_compile_definitions(dyn_array_tests PRIVATE UNITY_INCLUDE_DOUBLE_SUPPORT)

add_test(NAME dyn_array_tests COMMAND dyn_array_tests)

########################################
# String Tests
########################################
set(STRING_TEST_SRC
    test_string.c
    ${UNITY_DIR}/src/unity.c
)

add_executable(string_tests ${STRING_TEST_SRC})

target_include_directories(string_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/data-structures/
    ${UNITY_DIR}/src
)

target_compile_definitions(string_tests PRIVATE UNITY_INCLUDE_DOUBLE_SUPPORT)

add_test(NAME string_tests COMMAND string_tests)

########################################
# Hash Map Tests
########################################
set(STRING_TEST_SRC
    test_hash_map.c
    ${UNITY_DIR}/src/unity.c
)

add_executable(hash_map_tests ${STRING_TEST_SRC})

target_include_directories(hash_map_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/data-structures/
    ${UNITY_DIR}/src
)

target_compile_definitions(hash_map_tests PRIVATE UNITY_INCLUDE_DOUBLE_SUPPORT)

add_test(NAME hash_map_tests COMMAND hash_map_tests)