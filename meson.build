project('MMV',
        'cpp',
        version: '0.1',
        default_options : ['cpp_std=c++23', 'warning_level=1']
        )
subdir('src')
includeDir = include_directories('include')
dep = [dependency('glm'), dependency('stb')]
executable('mmv', sources, include_directories: includeDir, dependencies: dep)
