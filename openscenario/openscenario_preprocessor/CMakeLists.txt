cmake_minimum_required(VERSION 3.16.3) # Ubuntu 20.04 default CMake version

project(openscenario_preprocessor)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
find_package(rclcpp REQUIRED)
find_package(openscenario_interpreter REQUIRED)
find_package(openscenario_preprocessor_msgs REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
pkg_check_modules(XcercesC xerces-c)

include_directories(${XcercesC_INCLUDE_DIRS})
include_directories(include)

add_library(${PROJECT_NAME} SHARED src/${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} ${XcercesC_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} glog)
target_include_directories(${PROJECT_NAME} PUBLIC ${openscenario_interpreter_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC -DWITHOUT_ROS)

find_library(interpreter_without_ros NAMES openscenario_interpreter_without_ros)

add_executable(${PROJECT_NAME}_command src/${PROJECT_NAME}_command.cpp)
target_link_libraries(${PROJECT_NAME}_command ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_command ${interpreter_without_ros})
target_link_libraries(${PROJECT_NAME}_command ${Boost_LIBRARIES})
install(TARGETS ${PROJECT_NAME}_command
  DESTINATION lib/${PROJECT_NAME}
  INCLUDES DESTINATION include/${PROJECT_NAME})

add_executable(${PROJECT_NAME}_node src/${PROJECT_NAME}_node.cpp)
target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME})
ament_target_dependencies(${PROJECT_NAME}_node
  rclcpp
  openscenario_interpreter
  openscenario_preprocessor_msgs
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
