add_executable(roundtrip roundtrip.cc)
target_link_libraries(roundtrip xnet_net)

add_executable(roundtrip_udp roundtrip_udp.cc)
target_link_libraries(roundtrip_udp xnet_net)

