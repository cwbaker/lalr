project(LALR)

### ---------------------- Static Library  ---------------------  ###
file(GLOB SOURCES "./src/lalr/*.cpp")
file(GLOB HEADERS "./src/lalr/*.hpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ./src/)

### -------------------------- offline compiler  --------------------------  ###
file(GLOB COMPILER_SOURCES "./src/lalr/lalrc/*.cpp")
add_executable(${PROJECT_NAME}Compiler ${COMPILER_SOURCES})
target_link_libraries(${PROJECT_NAME}Compiler ${PROJECT_NAME})
 
### -------------------------- Tests  --------------------------  ###
add_subdirectory(src/unittest-cpp)

enable_testing()
file(GLOB TEST_SOURCES "./src/lalr/lalr_test/*.cpp")
add_executable(${PROJECT_NAME}Tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}Tests ${PROJECT_NAME} UnitTest++)
add_test(NAME ${PROJECT_NAME}Tests COMMAND ${PROJECT_NAME}Tests)