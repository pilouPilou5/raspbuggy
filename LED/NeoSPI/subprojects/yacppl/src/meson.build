src = [
  'Logger.cpp',
  'LoggerSettings.cpp',
]

deps = [
   dependency('yaml-cpp'),
]

inc = include_directories('include')

lib = shared_library('YACPPL', src, dependencies : deps, install: true, include_directories : inc)
