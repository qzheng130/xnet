add_library(xnet_curl Curl.cc)
target_link_libraries(xnet_curl xnet_net)
target_link_libraries(xnet_curl curl)

install(TARGETS xnet_curl DESTINATION lib)
install(FILES Curl.h DESTINATION include/xnet/curl)

add_executable(mcurl mcurl.cc)
target_link_libraries(mcurl xnet_curl)

add_executable(curl_download download.cc)
target_link_libraries(curl_download xnet_curl)

