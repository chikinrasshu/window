chk_window_src = ['chk/window/window.c']
chk_window_deps = [dependency('chk_common')]

chk_window_dep = library(
    'chk_window',
    sources: chk_window_src,
    include_directories: chk_window_inc,
    dependencies: chk_window_deps,

    gnu_symbol_visibility: 'hidden',
    c_shared_args: ['-DCHK_WINDOW_DLL', '-DCHK_WINDOW_DLL_EXPORTS'],
    install: true,
)