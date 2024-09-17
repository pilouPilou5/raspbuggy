project('YACPPL', ['cpp'],
          version: '0.0.1',
    meson_version: '>= 0.62.0',
  default_options: [ 'warning_level=2', 'werror=false', 'cpp_std=c++17', ],
)

subdir('src')
subdir('example')

proj_dep = declare_dependency(compile_args : ['-std=c++17'], include_directories : inc, link_with : lib, dependencies : deps)
