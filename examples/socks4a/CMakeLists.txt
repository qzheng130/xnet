add_executable(balancer balancer.cc)
target_link_libraries(balancer xnet_net)

add_executable(tcprelay tcprelay.cc)
target_link_libraries(tcprelay xnet_net)

add_executable(socks4a socks4a.cc)
target_link_libraries(socks4a xnet_net)

