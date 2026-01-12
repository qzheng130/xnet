set(http_SRCS
  HttpServer.cc
  HttpResponse.cc
  HttpContext.cc
  )

add_library(xnet_http ${http_SRCS})
target_link_libraries(xnet_http xnet_net)

install(TARGETS xnet_http DESTINATION lib)
set(HEADERS
  HttpContext.h
  HttpRequest.h
  HttpResponse.h
  HttpServer.h
  )
install(FILES ${HEADERS} DESTINATION include/xnet/net/http)

if(XNET_BUILD_EXAMPLES)
add_executable(httpserver_test tests/HttpServer_test.cc)
target_link_libraries(httpserver_test xnet_http)

if(BOOSTTEST_LIBRARY)
add_executable(httprequest_unittest tests/HttpRequest_unittest.cc)
target_link_libraries(httprequest_unittest xnet_http boost_unit_test_framework)
endif()

endif()

# add_subdirectory(tests)
