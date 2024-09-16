standarttest_sources = [
  'main2.cpp',
]

if not meson.is_subproject()
   executable('standarttest2', standarttest_sources, install: false, link_with : lib, include_directories : inc, dependencies : deps)
endif
