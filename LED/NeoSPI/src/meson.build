neospi_sources = [
  'NeoSPI.cpp',
  'D_RACE_LIB_LED.cpp'
]

deps = [
  yacppl,
]

inc = include_directories('include')

lib = shared_library('led', neospi_sources, install: true, include_directories : inc, dependencies : deps)
