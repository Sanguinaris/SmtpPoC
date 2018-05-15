cmake_minimum_required(VERSION 3.7.2)
project(SmtpTest0r VERSION 1.0.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FindThreads)
include(ExternalProject)

include_directories(include)

set(ASIO_DIR ${CMAKE_BINARY_DIR}/asio)

option(BUILD_OFFLINE "Dont try to download dependencies" OFF)

if(${BUILD_OFFLINE})
    set_property(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} PROPERTY EP_UPDATE_DISCONNECTED ON)
endif()

ExternalProject_Add( asio
    SOURCE_DIR  ${ASIO_DIR}
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG master
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

add_executable(SmtpTest0r				src/MainEntry.cpp
                src/SMTPSock.cpp		include/SMTPSock.h
				src/ConsoleLogger.cpp	include/ConsoleLogger.h
				include/ILogger.h)
                
set_target_properties(SmtpTest0r    PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED YES)
                        
add_dependencies(SmtpTest0r asio)

target_compile_definitions(SmtpTest0r PUBLIC ASIO_STANDALONE)

include_directories(include ${ASIO_DIR}/asio/include)
