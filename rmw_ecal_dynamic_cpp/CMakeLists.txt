# ========================= RMW eCAL LICENSE =================================
#
# Copyright (C) 2019 - 2020 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= RMW eCAL LICENSE =================================

cmake_minimum_required(VERSION 3.12)

project(rmw_ecal_dynamic_cpp)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(/W4)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

find_package(rmw REQUIRED)
find_package(rmw_ecal_shared_cpp REQUIRED)
find_package(rosidl_generator_c REQUIRED)
find_package(rosidl_typesupport_introspection_c REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

find_package(ament_cmake REQUIRED)

find_package(eCAL REQUIRED)

include_directories(
  src
)

add_library(${PROJECT_NAME} SHARED
	src/rmw/event.cpp
	src/rmw/get_network_flow_endpoints.cpp
	src/rmw/get_node_info_and_types.cpp
	src/rmw/get_service_names_and_types.cpp
	src/rmw/get_topic_names_and_types.cpp
	src/rmw/get_topic_endpoint_info.cpp
	src/rmw/init_options.cpp
	src/rmw/init.cpp
	src/rmw/qos_profiles.cpp
	src/rmw/rmw.cpp
	src/rmw/features.cpp
	src/serialization/serializer_cpp.cpp
	src/serialization/serializer_c.cpp
	src/serialization/deserializer_cpp.cpp
	src/serialization/deserializer_c.cpp
)

ament_export_dependencies(rmw)
ament_export_dependencies(rmw_ecal_shared_cpp)
ament_export_dependencies(rosidl_generator_c)
ament_export_dependencies(rosidl_typesupport_introspection_c)
ament_export_dependencies(rosidl_typesupport_introspection_cpp)

ament_target_dependencies(${PROJECT_NAME} 
	rmw 
	rmw_ecal_shared_cpp
	rosidl_typesupport_introspection_cpp
)

target_link_libraries(${PROJECT_NAME} 
	eCAL::core
)

configure_rmw_library(${PROJECT_NAME})

register_rmw_implementation(
	"c:rosidl_typesupport_c:rosidl_typesupport_introspection_c"
	"cpp:rosidl_typesupport_cpp:rosidl_typesupport_introspection_cpp"
)

ament_export_libraries(${PROJECT_NAME})

install(
  TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_package()
