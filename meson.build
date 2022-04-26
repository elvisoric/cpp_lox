
project('lox', 'cpp', default_options:['cpp_std=c++17'])

srcs = files(['srcs/main.cpp'])

incdir = include_directories('include')
fmt_dep = dependency('fmt')
project_dep = declare_dependency(include_directories:incdir, sources:srcs, dependencies:[fmt_dep])

executable('lox', dependencies:[project_dep])

