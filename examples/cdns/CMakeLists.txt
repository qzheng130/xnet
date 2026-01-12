add_library(xnet_cdns Resolver.cc)
target_link_libraries(xnet_cdns xnet_net)
target_link_libraries(xnet_cdns cares)

install(TARGETS xnet_cdns DESTINATION lib)
install(FILES Resolver.h DESTINATION include/xnet/cdns)

add_executable(cdns dns.cc)
target_link_libraries(cdns xnet_cdns)

