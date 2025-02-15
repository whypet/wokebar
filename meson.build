project('wokebar', 'c', default_options: ['c_std=c90'])

deps = [ dependency('xcb') ]
src = files(
  'wokebar.c',
  'xwindow.c'
)
# incdir = include_directories('.')

executable('wokebar', dependencies: deps, sources: src)
