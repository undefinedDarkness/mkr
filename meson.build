project('mkr', 'c')

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

src = [
'src/modes/math.c',
'src/modes/generic.c',
'src/modes/script.c',
'src/modes/todo.c',
'src/modes/dashboard.c',
'src/modes/font.c',
'src/mode-change.c',
'src/insertion.c',
'src/execute.c',
'src/sort.c',
'src/modes/dl.c',
'src/app.c',
'src/layout.c',
'src/modes/app.c',
'src/selection.c',
  'src/modes/emoji.c',
resources
]

executable('mkr', sources : src, dependencies : deps, install : true)
