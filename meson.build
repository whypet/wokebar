project('wokebar', 'c', default_options : ['c_std=c89', 'warning_level=3'])

deps = [ dependency('xcb') ]
src = files(
  'wokebar.c',
  'xwindow.c'
)
# incdir = include_directories('.')

executable('wokebar', dependencies : deps, sources : src)
