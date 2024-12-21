project(
    'chk_window',
    'c',
    version: '0.1.0',
    meson_version: '>=1.3.0',
    license: 'MIT',
)

# Library
subdir('include')
subdir('src')

# Pkg-Config
pkg = import('pkgconfig')
pkg.generate(chk_window_dep)