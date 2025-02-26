project('toml-c', 'c', default_options : ['c_std=c99', 'warning_level=3'])

libtoml = static_library(
	'toml-c',
	sources : files('toml.c', 'toml.h')
)

libtoml_dep = declare_dependency(
	include_directories : include_directories('.'),
	link_with : libtoml
)
