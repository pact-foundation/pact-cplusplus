cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cpp-consumer VERSION 0.1.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Initialize Conan
if(PactUseConan)
	if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
		include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
		conan_basic_setup()
	else()
		message(STATUS "Conan disabled or build info not found; continuing without Conan")
	endif()
endif()

add_library(pact-cpp-consumer STATIC consumer.cpp matchers.cpp)

target_include_directories(pact-cpp-consumer
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  PRIVATE
    ${PACT_FFI_INCLUDE}
)

# Link deps privately so they aren’t required by client apps
target_link_libraries(pact-cpp-consumer PRIVATE "${PACT_FFI_LIB}" nlohmann_json::nlohmann_json)

if(PactUseConan)
    TARGET_LINK_LIBRARIES(pact-cpp-consumer ${CONAN_LIBS})
endif()