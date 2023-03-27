project('mkr', 'c', default_options: [ 'c_std=c2x' ])

# Add Math Library
cc = meson.get_compiler('c')

deps = [ dependency('gtk+-3.0'), dependency('x11'), cc.find_library('m', required: false) ]

gnome = import('gnome')
 resources = gnome.compile_resources(
'mkr-resources',
   'resources/index.xml',
   source_dir: 'resources',
   c_name: 'mkr'
 )

lib = [
  'lib/match.c'
  ]

src = [
'src/modes/upload.c',
'src/help.c',
'src/modes/hermes.c',
'src/modes/math.c',
'src/modes/generic.c',
'src/modes/script.c',
'src/modes/todo.c',
'src/modes/dashboard.c',
'src/modes/font.c',
'src/mode-change.c',
'src/insertion.c',
'src/help.c',
'src/execute.c',
'src/sort.c',
'src/modes/dl.c',
'src/app.c',
'src/layout.c',
'src/modes/app.c',
'src/selection.c',
  'src/modes/emoji.c',
resources,
lib
]

executable('mkr', sources : src, dependencies : deps, install : true, c_args: '-Wno-incompatible-pointer-types')
