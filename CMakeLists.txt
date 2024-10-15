cmake_minimum_required(VERSION 3.5)
project(ConcreteScript VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)

set(CSI_PUBLIC_HEADERS
        Interpreter/include/hello.h
)
set(CSI_SOURCES
        Interpreter/src/hello.c
)

set(CSVS_SOURCES
        VisualScripting/src/main.c
)

add_executable(ConcreteScriptInterpreter ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES} Interpreter/src/main.c)
if (BUILD_SHARED_LIBS)
    add_library(ConcreteScriptInterpreterLib SHARED ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES})
else()
    add_library(ConcreteScriptInterpreterLib STATIC ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES})
endif()
add_executable(ConcreteScriptVisualScripting ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES} ${CSVS_SOURCES})

target_include_directories(ConcreteScriptInterpreter PRIVATE Interpreter/src Interpreter/include)
target_include_directories(ConcreteScriptInterpreterLib PRIVATE Interpreter/src)
target_include_directories(ConcreteScriptInterpreterLib PUBLIC Interpreter/include)
target_include_directories(ConcreteScriptVisualScripting PRIVATE Interpreter/src Interpreter/include VisualScripting/src)
