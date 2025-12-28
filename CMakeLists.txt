cmake_minimum_required(VERSION 3.20)
project(tc_validator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(creditcard
    src/CreditCard.cpp
)

target_include_directories(creditcard
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(creditcard PUBLIC cxx_std_20)

add_executable(tc_validator
    src/main.cpp
)

target_link_libraries(tc_validator PRIVATE creditcard)

add_executable(tc_validator_tests
    tests/test_creditcard.cpp
)

target_link_libraries(tc_validator_tests PRIVATE creditcard)

enable_testing()
add_test(NAME tc_validator_tests COMMAND tc_validator_tests)
