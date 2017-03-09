# rzeczy do zmiany
set(LOCAL_PROJECT_NAME MatlabExchange_Test)
# test case kopiowne z source do binary
configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/magic3.dat 
				${CMAKE_CURRENT_BINARY_DIR}/magic3.dat
				COPYONLY)


project(${LOCAL_PROJECT_NAME})
# get directory prefixes for libs and includes of external tools
IF (BUILD_GTEST)
	ExternalProject_Get_Property(googletest binary_dir)
	set(GTest_LIB ${binary_dir}/googlemock/gtest)
	ExternalProject_Get_Property(googletest source_dir)
	set(GTest_SRC ${source_dir}/googletest) 
	set(GTest_DEP googletest)
ENDIF()

include_directories(${GTest_SRC}/include ${PBToolset_SOURCE_DIR}/include)
add_executable(${LOCAL_PROJECT_NAME} C_MatlabExchange_Tests.cpp)
# gtest specific
target_compile_options(${LOCAL_PROJECT_NAME} PUBLIC -D_VARIADIC_MAX=10)

target_link_libraries(${LOCAL_PROJECT_NAME} ${GTest_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX} ${GTest_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})

# linking with tested lib
target_link_libraries(${LOCAL_PROJECT_NAME} MatlabExchange -pthread)
add_dependencies(${LOCAL_PROJECT_NAME} ${GTest_DEP} MatlabExchange)
add_test(${LOCAL_PROJECT_NAME} ${LOCAL_PROJECT_NAME})

#-- GTest_LIB /home/baniuk/Documents/Repos/PBToolset/bin/External_dep/googletest-prefix/src/googletest-build/googletest
#-- GTest_SRC /home/baniuk/Documents/Repos/PBToolset/bin/External_dep/googletest-prefix/src/googletest/googletest

