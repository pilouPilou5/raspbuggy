project('NeoSPI', 'cpp', default_options : ['cpp_std=c++17'], version: '0.1.0',)

yacppl = dependency('yacppl', fallback : ['yacppl', 'proj_dep'])

subdir('src') 
subdir('example')

proj_dep = declare_dependency(compile_args : ['-std=c++17'], include_directories : inc, link_with : lib, dependencies : deps)
