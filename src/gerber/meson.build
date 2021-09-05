gerb_sources = [
		'amacro.c', 'amacro.h',
		'common.h',
		'csv.c', 'csv.h', 'csv_defines.h',
		'draw.c', 'draw.h',
		'drill.c', 'drill.h',
		'drill_stats.c', 'drill_stats.h',
		'export-drill.c',
		'export-geda-pcb.c',
		'export-image.c',
		'export-isel-drill.c',
		'export-rs274x.c',
		'gerb_file.c', 'gerb_file.h',
		'gerb_image.c', 'gerb_image.h',
		'gerb_stats.c', 'gerb_stats.h',
		'gerber.c', 'gerber.h',
		'gerbv.c', 'gerbv.h',
		'gerbv_icon.h',
		'gettext.h',
		'pick-and-place.c', 'pick-and-place.h',
		'selection.c', 'selection.h',
		'tooltable.c'
]

#GTK Dependencies
gtk_dep = dependency('gtk4')
glib_dep = dependency('glib-2.0', version: '>= 2.38')
gobject_dep = dependency('gobject-2.0', version: '>= 2.38')
gmodule_dep = dependency('gmodule-2.0')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

deps = [
        glib_dep,
        gobject_dep,
        gmodule_dep,
        gtk_dep,
        m_dep,
       ]

gerb_lib_static = static_library('gerb',
                            gerb_sources,
                            dependencies : deps,
                            install : true)

# We use libtool-version numbers because it's easier to understand.
# Before making a release, the libruntime_so_*
# numbers should be modified. The components are of the form C:R:A.
# a) If binary compatibility has been broken (eg removed or changed interfaces)
#    change to C+1:0:0.
# b) If interfaces have been changed or added, but binary compatibility has
#    been preserved, change to C+1:0:A+1
# c) If the interface is the same as the previous version, change to C:R+1:A
libruntime_lt_c=1
libruntime_lt_r=0
libruntime_lt_a=0

libruntime_so_version = '@0@.@1@.@2@'.format((libruntime_lt_c - libruntime_lt_a),
                                              libruntime_lt_a,
                                              libruntime_lt_r)

gerb_lib_shared = shared_library('gerb',
                            gerb_sources,
                            version : libruntime_so_version,
                            dependencies : deps,
                            install : true)
