cmake_minimum_required(VERSION 3.9)

project(gerbv VERSION 1.0.0 DESCRIPTION "gerber viewer library")

find_package(PkgConfig REQUIRED)

pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 cairo pango)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFiles)

check_include_files(unistd.h HAVE_UNISTD_H)

configure_file(${PROJECT_SOURCE_DIR}/cmake/config.h.cmake ${PROJECT_BINARY_DIR}/include/config.h)
add_definitions(-DHAVE_CONFIG_H)

include_directories("${PROJECT_BINARY_DIR}/include")

option(WITH_RENDER_CAIRO "Render with cairo" OFF)

if(WITH_RENDER_CAIRO)
  add_definitions(-DRENDER_WITH_CAIRO)
  set(WITH_RENDER_CAIRO ON)
  set(CAIRO_FILES
  )
endif()

add_library(gerbv STATIC
		amacro.c
		csv.c
                draw.c
                export-image.c
		drill.c
		drill_stats.c
		export-drill.c
		export-geda-pcb.c
		export-isel-drill.c
		export-rs274x.c
		gerb_file.c
		gerb_image.c
		gerb_stats.c
		gerber.c
		gerbv.c
		pick-and-place.c
		selection.c
		tooltable.c
                ${CAIRO_FILES})

if(WITH_RENDER_CAIRO)
endif()

target_link_libraries(gerbv PkgConfig::deps)

