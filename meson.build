project('wokebar', 'c', default_options : ['c_std=c99', 'warning_level=3'])

toml_c = subproject('toml-c')
libtoml_dep = toml_c.get_variable('libtoml_dep')

deps = [
  dependency('fontconfig'),
  dependency('freetype2'),
  dependency('xcb'),
  libtoml_dep,
]

src = files(
  'conf.c',
  'font.c',
  'wokebar.c',
  'xwindow.c',
)

executable('wokebar', dependencies : deps, sources : src)
