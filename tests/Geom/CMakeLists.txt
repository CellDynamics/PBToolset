# rzeczy do zmiany
set(LOCAL_PROJECT_NAME Geom_Test)
# test case kopiowne z source do binary
#configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/magic3.dat 
#				${CMAKE_CURRENT_BINARY_DIR}/magic3.dat
#				COPYONLY)


project(${LOCAL_PROJECT_NAME})
# get directory prefixes for libs and includes of external tools (if local build of gtest)
IF (BUILD_GTEST)
	ExternalProject_Get_Property(googletest binary_dir)
	set(GTest_LIB ${binary_dir}/googlemock/gtest)
	ExternalProject_Get_Property(googletest source_dir)
	set(GTest_SRC ${source_dir}/googletest) 
	set(GTest_DEP googletest)
ENDIF()

include_directories(${GTest_SRC}/include ${PBToolset_SOURCE_DIR}/include)
add_executable(${LOCAL_PROJECT_NAME} GTest_starter.cpp C_Point_Tests.cpp C_Vector_Tests.cpp C_Line_Tests.cpp)

# gtest specific
target_compile_options(${LOCAL_PROJECT_NAME} PUBLIC -D_VARIADIC_MAX=10)
# zmienna COMMON_EXT_LIB_OUTPUT jest ustawiana albo na poczatku albo pochodzi z nadrzędnego projektu i dotyczy tylko zewnętrznych i wspólnych projektów
target_link_libraries(${LOCAL_PROJECT_NAME} ${GTest_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX} ${GTest_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})

# linking with tested lib
target_link_libraries(${LOCAL_PROJECT_NAME} Geom -pthread)
add_dependencies(${LOCAL_PROJECT_NAME} ${GTest_DEP} Geom)

add_test(${LOCAL_PROJECT_NAME} ${LOCAL_PROJECT_NAME})
