
if(NOT DEFINED ENV{COMMONLIB_SSE_FOLDER})
  message(FATAL_ERROR "Missing COMMONLIB_SSE_FOLDER environment variable")
endif()

if(NOT DEFINED ENV{IMGUI_LIB_FOLDER})
  message(FATAL_ERROR "Missing IMGUI_LIB_FOLDER environment variable")
endif()

if(NOT DEFINED ENV{SKYRIM_MODS_FOLDER})
  message(FATAL_ERROR "Missing SKYRIM_MODS_FOLDER environment variable")
endif()


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(wildlander_output false)
set(steam_owrt_output false)
set(steam_mods_output true)

# It's recommended to set a minimum CMake version.
# If you use CMake features from higher versions, update this to match.
cmake_minimum_required(VERSION 3.21)
# Set your project name. This will be the name of your SKSE .dll file.
set(AUTHOR_NAME "AuthorName")
set(PRODUCT_NAME "SKSEMenuFrameworkTemplate0.2")
set(BEAUTIFUL_NAME "SKSE Menu Framework Template")

project(${PRODUCT_NAME} VERSION 0.2.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)




configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)

set(RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
set(RESOURCE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.res)

add_custom_command(
    OUTPUT ${RESOURCE_OUTPUT}
    COMMAND rc /fo ${RESOURCE_OUTPUT} ${RESOURCE_FILE}
    DEPENDS ${RESOURCE_FILE}
)

# Add a custom target to ensure the resource file is generated
add_custom_target(Resource ALL DEPENDS ${RESOURCE_OUTPUT})



# Drew W � 18.01.2024 02:44: Here is my CMakeLists.txt that uses FetchContent for NG
include(FetchContent)

set(ENABLE_SKYRIM_SE ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_VR ON CACHE BOOL " " FORCE)
set(BUILD_TESTS OFF CACHE BOOL " " FORCE)

#FetchContent_MakeAvailable(CommonLibNG)


add_subdirectory("$ENV{COMMONLIB_SSE_FOLDER}" ${CMAKE_CURRENT_BINARY_DIR}/_deps/clib-build)
get_target_property(COMMONLIB_SRC_DIR CommonLibSSE SOURCE_DIR)

# CLIB-UTIL
find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/utils.hpp")

file(
  GLOB_RECURSE
  sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${headers} ${sources}
)

set(BUILD_TESTS OFF)
include(${COMMONLIB_SRC_DIR}/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(
  ${PROJECT_NAME}
  AUTHOR AUTHOR_NAME
  SOURCES ${headers} ${sources}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23) # <--- use C++23 standard
target_precompile_headers(${PROJECT_NAME} PRIVATE include/PCH.h) # <--- PCH.h is required!
target_include_directories(
	${PROJECT_NAME}
	PRIVATE
	${CLIB_UTIL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")



function(copyOutputs TARGET_FOLDER)
    # If you specify an <OUTPUT_FOLDER> (including via environment variables)
    # then we'll copy your mod files into Skyrim or a mod manager for you!

    # Copy the SKSE plugin .dll files into the SKSE/Plugins/ folder
    set(DLL_FOLDER "${TARGET_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    # If you perform a "Debug" build, also copy .pdb file (for debug symbols)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endfunction()


if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    copyOutputs("$ENV{SKYRIM_FOLDER}/Data")
endif()
if(DEFINED ENV{WILDLANDER_OWRT_FOLDER} AND IS_DIRECTORY "$ENV{WILDLANDER_OWRT_FOLDER}" AND wildlander_output)
    copyOutputs("$ENV{WILDLANDER_OWRT_FOLDER}/${BEAUTIFUL_NAME}")
endif()
if(DEFINED ENV{SKYRIM_OWRT_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_OWRT_FOLDER}" AND steam_owrt_output)
    copyOutputs("$ENV{SKYRIM_OWRT_FOLDER}/${BEAUTIFUL_NAME}")
endif()
if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}" AND steam_mods_output)
    copyOutputs("$ENV{SKYRIM_MODS_FOLDER}/${BEAUTIFUL_NAME}")
endif()
if(DEFINED ENV{SKYRIM_MODS_FOLDER2} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER2}" AND steam_mods_output)
    copyOutputs("$ENV{SKYRIM_MODS_FOLDER2}/${BEAUTIFUL_NAME}")
endif()