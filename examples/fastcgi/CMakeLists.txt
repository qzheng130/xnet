add_executable(fastcgi_test fastcgi.cc fastcgi_test.cc ../sudoku/sudoku.cc)
target_link_libraries(fastcgi_test xnet_net)

