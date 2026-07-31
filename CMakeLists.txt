cmake_minimum_required(VERSION 3.21)
project(BridgeABI VERSION 0.1.0 LANGUAGES CXX)

option(BRIDGEABI_BUILD_EXAMPLES "Build BridgeABI examples" ON)
option(BRIDGEABI_BUILD_TESTS "Build BridgeABI tests" ON)
option(BRIDGEABI_BUILD_GENERATOR "Build the BridgeABI IDL generator" ON)

add_library(bridgeabi src/loader.cpp src/manifest.cpp)
add_library(BridgeABI::bridgeabi ALIAS bridgeabi)
target_compile_features(bridgeabi PUBLIC cxx_std_20)
target_include_directories(bridgeabi PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_definitions(bridgeabi PRIVATE BRIDGEABI_BUILDING_LIBRARY=1)
if(MSVC)
  target_compile_options(bridgeabi PRIVATE /W4 /permissive-)
else()
  target_compile_options(bridgeabi PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
endif()
if(UNIX AND NOT APPLE)
  target_link_libraries(bridgeabi PRIVATE ${CMAKE_DL_LIBS})
endif()

if(BRIDGEABI_BUILD_GENERATOR)
  add_executable(bridgeabi_cli generator/cli/main.cpp generator/parser/idl_parser.cpp generator/semantics/validate.cpp generator/compatibility/snapshot.cpp generator/emitters/c_header.cpp generator/emitters/artifacts.cpp)
  set_target_properties(bridgeabi_cli PROPERTIES OUTPUT_NAME bridgeabi)
  target_compile_features(bridgeabi_cli PRIVATE cxx_std_20)
  target_include_directories(bridgeabi_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generator)
  target_link_libraries(bridgeabi_cli PRIVATE BridgeABI::bridgeabi)
endif()

if(BRIDGEABI_BUILD_EXAMPLES)
  add_subdirectory(examples/greeter)
endif()

if(BRIDGEABI_BUILD_TESTS)
  enable_testing()
  add_executable(bridgeabi_unit_tests tests/unit/abi_tests.cpp)
  target_link_libraries(bridgeabi_unit_tests PRIVATE BridgeABI::bridgeabi)
  add_test(NAME bridgeabi.unit COMMAND bridgeabi_unit_tests)
  add_test(NAME bridgeabi.greeter COMMAND bridgeabi_greeter_host $<TARGET_FILE:bridgeabi_greeter_plugin>)
  add_executable(bridgeabi_generator_tests tests/unit/generator_tests.cpp generator/parser/idl_parser.cpp generator/semantics/validate.cpp generator/compatibility/snapshot.cpp generator/emitters/c_header.cpp generator/emitters/artifacts.cpp)
  target_include_directories(bridgeabi_generator_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generator)
  target_link_libraries(bridgeabi_generator_tests PRIVATE BridgeABI::bridgeabi)
  add_test(NAME bridgeabi.generator COMMAND bridgeabi_generator_tests)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
install(TARGETS bridgeabi EXPORT BridgeABITargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT BridgeABITargets NAMESPACE BridgeABI:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BridgeABI)
install(FILES cmake/BridgeABIGenerate.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BridgeABI)
configure_package_config_file(cmake/BridgeABIConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/BridgeABIConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BridgeABI)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/BridgeABIConfigVersion.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BridgeABIConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/BridgeABIConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BridgeABI)
