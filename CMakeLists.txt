# CMake build script for PBToolset

cmake_minimum_required (VERSION 3.0.2)

find_package(Subversion REQUIRED)

# do ustawienia
set(ROOT_PROJECT_NAME "PBToolset")

project(${ROOT_PROJECT_NAME})

IF(WIN32)
	enable_language(RC)
ENDIF()

# If these variables exist (passed from external project) local External Project will not be build
IF(GTest_LIB AND GTest_SRC)
	set(BUILD_GTEST FALSE)
	message("Using external build of Gtest")
ELSE()
	set(BUILD_GTEST TRUE)
	message("Using local build of Gtest")
ENDIF()
# If these variables are set in env build is performed offline:
#	GTest_GIT_REPOSITORY - any value (gtest must be in /external_dep/gtest)
IF(DEFINED ENV{GTest_GIT_REPOSITORY})
	set(GTest_GIT_REPOSITORY $ENV{GTest_GIT_REPOSITORY})
	message("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	message("Use local gtest repo " External_dep/gtest)
	message("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
ELSE ()
	set(GTest_GIT_REPOSITORY https://github.com/google/googletest.git) 
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# create lib output directory that can be removed by clean-build
file(MAKE_DIRECTORY lib)
# create doc output directory that can be removed by clean-build
file(MAKE_DIRECTORY doc)

include(ExternalProject)
add_subdirectory(External_dep)

# additional subdirs are there - see src dir
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)