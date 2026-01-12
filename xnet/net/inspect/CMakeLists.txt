set(inspect_SRCS
  Inspector.cc
  PerformanceInspector.cc
  ProcessInspector.cc
  SystemInspector.cc
  )

add_library(xnet_inspect ${inspect_SRCS})
target_link_libraries(xnet_inspect xnet_http)

if(TCMALLOC_INCLUDE_DIR AND TCMALLOC_LIBRARY)
  set_target_properties(xnet_inspect PROPERTIES COMPILE_FLAGS "-DHAVE_TCMALLOC")
  target_link_libraries(xnet_inspect tcmalloc_and_profiler)
endif()

install(TARGETS xnet_inspect DESTINATION lib)
set(HEADERS
  Inspector.h
  )
install(FILES ${HEADERS} DESTINATION include/xnet/net/inspect)

if(XNET_BUILD_EXAMPLES)
add_executable(inspector_test tests/Inspector_test.cc)
target_link_libraries(inspector_test xnet_inspect)
endif()

