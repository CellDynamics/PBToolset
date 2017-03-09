set(LOCAL_PROJECT_NAME MatlabExchange)
project(${LOCAL_PROJECT_NAME})
# includes in source files must be relative to ./include dir. Includes specific to 
# given project are in separate subdirectories

include_directories(${PBToolset_SOURCE_DIR}/include)

add_library(${LOCAL_PROJECT_NAME} STATIC C_MatlabExchange.cpp)

IF(UNIX)
	target_compile_options(${LOCAL_PROJECT_NAME} PUBLIC -std=c++11 -Wall)
ENDIF()	