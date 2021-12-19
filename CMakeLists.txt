cmake_minimum_required(VERSION 3.14)
project(SojournerST)
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  sojournerst_test
  test_packet.cpp
)

target_include_directories(sojournerst_test PUBLIC 
                App/Inc
                Test/Stubs/Inc
)


target_link_libraries(
  sojournerst_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(sojournerst_test)