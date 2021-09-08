cmake_minimum_required(VERSION 3.9)

project(libgerbv VERSION 1.0.0 DESCRIPTION "gerber viewer library")

find_package(PkgConfig REQUIRED)

pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)

add_library(libgerbv STATIC
		amacro.c
		csv.c
		draw.c
		drill.c
		drill_stats.c
		export-drill.c
		export-geda-pcb.c
		export-image.c
		export-isel-drill.c
		export-rs274x.c
		gerb_file.c
		gerb_image.c
		gerb_stats.c
		gerber.c
		gerbv.c
		pick-and-place.c
		selection.c
		tooltable.c)

target_link_libraries(libgerbv PkgConfig::deps)

