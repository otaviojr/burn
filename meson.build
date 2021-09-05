project('burn', ['c'],
  version: '0.1',
  license: [
        'GPL-2.0',
  ],
  default_options : [ 'warning_level=1',
                      'buildtype=debugoptimized' ])

cc = meson.get_compiler('c')

sources = [
		'src/main.c',
                'src/presentation.c', 'src/presentation.h',
                'src/main_menu.c', 'src/main_menu.h',
          ]

#GTK Dependencies
gtk_dep = dependency('gtk4')

glib_dep = dependency('glib-2.0', version: '>= 2.38')
gobject_dep = dependency('gobject-2.0', version: '>= 2.38')
gmodule_dep = dependency('gmodule-2.0')
glibjson_dep = dependency('json-glib-1.0')

deps = [
        glib_dep,
        gobject_dep,
        gmodule_dep,
        glibjson_dep,
        gtk_dep,
       ]

cdata = configuration_data()
#check_headers = [
#  ['HAVE_INTTYPES_H', 'inttypes.h'],
#]

#foreach h : check_headers
#  if cc.has_header(h.get(1))
#    cdata.set(h.get(0), 1)
#  endif
#endforeach

#check_functions = [
  # check token HAVE_GETTEXT
#  ['HAVE_GETTEXT', 'gettext', true],
#]

#foreach f : check_functions
#  if cc.has_function(f.get(1))
#    cdata.set(f.get(0), 1)
#  else
#    if f.get(2) == true
#      error('@0@: not found'.format(f.get(1)))
#    endif
#  endif
#endforeach

cdata.set('PREFIX', '"@0@"'.format(get_option('prefix')))
cdata.set('LIBDIR', '"@0@"'.format(get_option('libdir')))
cdata.set('DATADIR', '"@0@"'.format(get_option('datadir')))

configure_file(output : 'config.h', configuration : cdata)

compiler = meson.get_compiler('c')

#code = '''
#  #include <stdio.h>
#  #include <lua.h>
#  #include <lualib.h>
#  #include <lauxlib.h>
#
#  int main(int argc, char **argv)
#  {
#    lua_State* l;
#    l = luaL_newstate();
#    luaL_openlibs(l);
#    if(luaL_dostring(l, "local http = require \"http.request\""))
#    {
#      printf("Couldn't load file: %s\n", lua_tostring(l, -1));
#      lua_close(l);
#      return -1;
#    }
#    lua_close(l);
#    printf("SUCCESS");
#    return 0;
#  }
#'''

#flags = run_command('pkg-config','--cflags','--libs','lua5.3')
#a_flags = flags.stdout().strip().split(' ')

#result = compiler.run(code, args : a_flags, name : 'lua5.3 has http plugin')
#if result.returncode() != 0 or result.compiled() == false
#  error('Lua HTTP support not installed: ' + result.stdout())
#endif

subdir('src/gerber')

exe = executable('burn', sources, dependencies : deps, link_with: gerb_lib_static, install : true)

ui_data_files = [
               'resources/ui/main.ui',
             ]

install_data(ui_data_files, install_dir : join_paths(get_option('datadir'),'burn','resources', 'ui'))

css_data_files = [
               'resources/css/main.css'
             ]

install_data(css_data_files, install_dir : join_paths(get_option('datadir'),'burn','resources', 'css'))

#fonts_data_files = [
#               'resources/fonts/material/MaterialIcons-Regular.ttf'
#             ]
#
#install_data(fonts_data_files, install_dir : join_paths('/usr','share','fonts', 'truetype', 'material'))

#drivers_data_files = [
#               'resources/drivers/FOSCAM_FI8918W.json',
#               'resources/drivers/FOSCAM_FI8918W.lua',
#               'resources/drivers/FOSCAM_FI9821P.json',
#               'resources/drivers/FOSCAM_FI9821P.lua'
#             ]
#install_data(drivers_data_files, install_dir : join_paths(get_option('datadir'),'gipcamviewer','drivers'))

test('Check Generated Application', exe)
