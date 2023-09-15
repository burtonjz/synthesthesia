cmake_minimum_required(VERSION 3.12)
project(synthesthesia)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2 -fPIC -ffast-math")

# Find and include libraries
find_package(PkgConfig REQUIRED)

pkg_check_modules(LV2 REQUIRED lv2)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(X11 REQUIRED x11)

#========================================================================
#=========================== SYNTHESTHESIA.SO ===========================
#========================================================================

# Source Files
set(SRC_DIR src)
file(GLOB SRC "${SRC_DIR}/*.cpp")

# Set SO
add_library(synthesthesia SHARED ${SRC})
set_target_properties(synthesthesia PROPERTIES PREFIX "")

# add package cflags
target_compile_options(synthesthesia PRIVATE
    ${LV2_CFLAGS}
)

# Include Directories
target_include_directories(synthesthesia PRIVATE 
    ${LV2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(synthesthesia PRIVATE 
    ${LV2_LIBRARIES}
    m
    stdc++
)

# Linker Flags
target_link_options(synthesthesia PRIVATE
    -fvisibility=hidden
    -Wl,-Bstatic
    -Wl,-Bdynamic
    -Wl,--as-needed
    -shared
    -pthread
)

#========================================================================
#========================= SYNTHESTHESIA-UI.SO ==========================
#========================================================================

# Source Files
set( UI_SRC_DIR src/ui-x11 )
set(UI_FILES
    "${CMAKE_SOURCE_DIR}/src/connection-manager.cpp"
)
file(GLOB UI_CPP_FILES
    "${UI_SRC_DIR}/*.cpp" 
    EXCLUDE "${UI_SRC_DIR}/test.cpp"    
)
list(APPEND UI_FILES ${UI_CPP_FILES})

# Set Executable
add_library(synthesthesia-ui SHARED ${UI_FILES})
set_target_properties(synthesthesia-ui PROPERTIES PREFIX "")

# add package cflags
target_compile_options(synthesthesia-ui PRIVATE
    ${LV2_CFLAGS}
    ${CAIRO_CFLAGS}
    ${X11_CFLAGS}
)

# Include Directories
target_include_directories(synthesthesia-ui PRIVATE 
    ${LV2_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/ui-x11
    ${CMAKE_SOURCE_DIR}/BWidgets/include
)

# Link libraries
target_link_libraries(synthesthesia-ui PRIVATE 
    ${LV2_LIBRARIES}
    m
    stdc++
    -lbwidgetscore
    -lcairoplus
    -lpugl
)

# Linker Flags
target_link_options(synthesthesia-ui PRIVATE
    -fvisibility=hidden
    -Wl,-Bstatic
    -Wl,-Bdynamic
    -Wl,--as-needed
    -shared
    -pthread
    -L${CMAKE_SOURCE_DIR}/BWidgets/build
    ${LV2_LDFLAGS}
    ${CAIRO_LDFLAGS}
    ${X11_LDFLAGS}
)


#========================================================================
#============================= MAKE PACKAGE =============================
#========================================================================

set(PACKAGE_FILES
    "${CMAKE_SOURCE_DIR}/src/manifest.ttl"
    "${CMAKE_SOURCE_DIR}/src/synthesthesia.ttl"
    "${CMAKE_BINARY_DIR}/synthesthesia.so"
    "${CMAKE_BINARY_DIR}/synthesthesia-ui.so"
)
file(GLOB ASSETS_FILES "${CMAKE_SOURCE_DIR}/assets/*.png")

set(INSTALL_DIR "$ENV{HOME}/.lv2")

add_custom_target(package
    DEPENDS ${PACKAGE_FILES}
    COMMENT "Packaging the LV2 Plugin"
)

add_custom_command(
    TARGET package
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/synthesthesia.lv2/assets 
    COMMAND ${CMAKE_COMMAND} -E copy ${PACKAGE_FILES} ${CMAKE_BINARY_DIR}/synthesthesia.lv2/ 
    COMMAND $(CMAKE_COMMAND) -E copy ${ASSETS_FILES} ${CMAKE_BINARY_DIR}/synthesthesia.lv2/assets/
)

# Install

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/synthesthesia.lv2
    DESTINATION ${INSTALL_DIR}
)