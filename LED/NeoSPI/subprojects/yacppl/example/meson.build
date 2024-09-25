example_sources = [
  'main.cpp',
]

if not meson.is_subproject()
   executable('example', example_sources, install: false, link_with : lib, include_directories : inc, dependencies : deps)
endif
