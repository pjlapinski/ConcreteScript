cmake_minimum_required(VERSION 3.5)
project(ConcreteScript VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)

set(CSI_PUBLIC_HEADERS
        interpreter/include/lexer.h
        interpreter/include/config.h
)
set(CSI_SOURCES
        interpreter/src/lexer.c
)

set(CSVS_SOURCES
        visual_scripting/src/main.c
)

add_executable(ConcreteScriptInterpreter ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES} interpreter/src/main.c)
if (BUILD_SHARED_LIBS)
    add_library(ConcreteScriptInterpreterLib SHARED ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES})
else()
    add_library(ConcreteScriptInterpreterLib STATIC ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES})
endif()
add_executable(ConcreteScriptVisualScripting ${CSI_PUBLIC_HEADERS} ${CSI_SOURCES} ${CSVS_SOURCES})

target_include_directories(ConcreteScriptInterpreter PRIVATE interpreter/src interpreter/include)
target_include_directories(ConcreteScriptInterpreterLib PRIVATE interpreter/src)
target_include_directories(ConcreteScriptInterpreterLib PUBLIC interpreter/include)
target_include_directories(ConcreteScriptVisualScripting PRIVATE interpreter/src interpreter/include visual_scripting/src)
