cmake_minimum_required(VERSION 3.20)

set(ProjectName SerializingHashMap)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${ProjectName} VERSION 1.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f5e592d8ee5ffb1d9af5be7f715ce3576b8bf9c4.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_FILES tests/*.cpp)
add_executable(${ProjectName} ${TEST_FILES})
target_include_directories(${ProjectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

target_link_libraries(
    ${ProjectName}
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${ProjectName})