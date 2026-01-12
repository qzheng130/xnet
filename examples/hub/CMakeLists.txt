add_executable(hub hub.cc codec.cc)
target_link_libraries(hub xnet_inspect)

add_library(xnet_pubsub pubsub.cc codec.cc)
target_link_libraries(xnet_pubsub xnet_net)

add_executable(pub pub.cc)
target_link_libraries(pub xnet_pubsub)

add_executable(sub sub.cc)
target_link_libraries(sub xnet_pubsub)

