add_executable(sudoku_solver_basic server_basic.cc sudoku.cc)
target_link_libraries(sudoku_solver_basic xnet_net)

add_executable(sudoku_solver_multiloop server_multiloop.cc sudoku.cc)
target_link_libraries(sudoku_solver_multiloop xnet_net)

add_executable(sudoku_solver_threadpool server_threadpool.cc sudoku.cc)
target_link_libraries(sudoku_solver_threadpool xnet_net)

add_executable(sudoku_solver_hybrid server_hybrid.cc sudoku.cc)
target_link_libraries(sudoku_solver_hybrid xnet_inspect)

add_executable(sudoku_solver_prod server_prod.cc sudoku.cc)
target_link_libraries(sudoku_solver_prod xnet_inspect)


add_executable(sudoku_client_batch batch.cc sudoku.cc)
target_link_libraries(sudoku_client_batch xnet_net)

add_executable(sudoku_client_pipeline pipeline.cc sudoku.cc)
target_link_libraries(sudoku_client_pipeline xnet_net)

add_executable(sudoku_loadtest loadtest.cc sudoku.cc)
target_link_libraries(sudoku_loadtest xnet_net)


if(BOOSTTEST_LIBRARY)
add_executable(sudoku_stat_unittest stat_unittest.cc)
target_link_libraries(sudoku_stat_unittest xnet_base boost_unit_test_framework)
endif()

