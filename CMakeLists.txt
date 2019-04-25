# cmake -DCMAKE_BUILD_TYPE=Debug .
# cmake -DCMAKE_BUILD_TYPE=Release .

cmake_minimum_required(VERSION 2.8)

project(ceres_simple_example)

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# helloworld
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} ${CERES_LIBRARIES})
