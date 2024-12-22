chk_window_test_dep = executable(
    'chk_window_test',
    ['chk/window_test/main.c'],
    include_directories: chk_window_inc,
    dependencies: chk_window_deps,
    link_with: [chk_window_dep],
)

test('chk_window_test', chk_window_test_dep)
