project('colr', 'c')

# Add Math Library
cc = meson.get_compiler('c')

deps = [ dependency('gtk+-3.0'), dependency('x11'), cc.find_library('m', required: false) ]

gnome = import('gnome')
# resources = gnome.compile_resources(
#   'colr-resources',
#   'resources/index.xml',
#   source_dir: 'resources',
#   c_name: 'colr'
# )

src = [
  'src/interact.c',
  'src/modes/dl.c',
  'src/app.c',
  'src/layout.c',
  'src/modes/app.c',
  'src/ui-behaviour.c'
  # resources
]

executable('colr', sources : src, dependencies : deps, install : true)
