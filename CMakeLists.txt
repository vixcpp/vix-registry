cmake_minimum_required(VERSION 3.20)

project(VixRegistry
  VERSION 0.0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(VIX_ENABLE_SANITIZERS "Enable ASan/UBSan (dev only)" OFF)
option(REGISTRY_BUILD_TESTS "Build tests" ON)
option(REGISTRY_BUILD_EXAMPLES "Build examples" OFF)
option(REGISTRY_USE_ORM "Enable Vix ORM (requires vix::orm in install)" ON)

find_package(vix QUIET CONFIG)
if (NOT vix_FOUND)
  find_package(Vix CONFIG REQUIRED)
endif()

set(REGISTRY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(REGISTRY_SRC_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(REGISTRY_CORE_SOURCES
  ${REGISTRY_SRC_DIR}/App.cpp

  ${REGISTRY_SRC_DIR}/http/HttpServer.cpp
  ${REGISTRY_SRC_DIR}/http/Routes.cpp
  ${REGISTRY_SRC_DIR}/http/Middleware.cpp

  ${REGISTRY_SRC_DIR}/domain/Package.cpp
  ${REGISTRY_SRC_DIR}/domain/Version.cpp
  ${REGISTRY_SRC_DIR}/domain/User.cpp
  ${REGISTRY_SRC_DIR}/domain/Token.cpp

  ${REGISTRY_SRC_DIR}/services/PackageService.cpp
  ${REGISTRY_SRC_DIR}/services/VersionService.cpp
  ${REGISTRY_SRC_DIR}/services/AuthService.cpp

  ${REGISTRY_SRC_DIR}/storage/LocalFileStorage.cpp
  ${REGISTRY_SRC_DIR}/storage/S3Storage.cpp

  ${REGISTRY_SRC_DIR}/db/Database.cpp
  ${REGISTRY_SRC_DIR}/db/PackageRepository.cpp
  ${REGISTRY_SRC_DIR}/db/UserRepository.cpp
)

add_library(registry_core STATIC ${REGISTRY_CORE_SOURCES})

target_include_directories(registry_core
  PUBLIC
    ${REGISTRY_INCLUDE_DIR}
)

target_link_libraries(registry_core
  PUBLIC
    vix::vix
)

if (REGISTRY_USE_ORM)
  if (TARGET vix::orm)
    target_link_libraries(registry_core PUBLIC vix::orm)
    target_compile_definitions(registry_core PUBLIC REGISTRY_USE_ORM=1)
    message(STATUS "Registry: ORM enabled (vix::orm)")
  elseif (TARGET Vix::orm)
    target_link_libraries(registry_core PUBLIC Vix::orm)
    target_compile_definitions(registry_core PUBLIC REGISTRY_USE_ORM=1)
    message(STATUS "Registry: ORM enabled (Vix::orm)")
  else()
    message(WARNING "REGISTRY_USE_ORM=ON but vix::orm target is not available. Building without ORM.")
    target_compile_definitions(registry_core PUBLIC REGISTRY_USE_ORM=0)
  endif()
else()
  target_compile_definitions(registry_core PUBLIC REGISTRY_USE_ORM=0)
endif()

if (MSVC)
  target_compile_options(registry_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(registry_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (VIX_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(registry_core PRIVATE -O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined)
  target_link_options(registry_core PRIVATE -fsanitize=address,undefined)
endif()

add_executable(registry ${REGISTRY_SRC_DIR}/main.cpp)
target_link_libraries(registry PRIVATE registry_core)

add_custom_target(run
  COMMAND $<TARGET_FILE:registry>
  DEPENDS registry
  USES_TERMINAL
)

if (REGISTRY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if (REGISTRY_BUILD_EXAMPLES)
  # add_subdirectory(examples)
endif()

