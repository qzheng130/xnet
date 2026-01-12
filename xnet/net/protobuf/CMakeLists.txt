add_library(xnet_protobuf_codec ProtobufCodecLite.cc)
set_target_properties(xnet_protobuf_codec PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(xnet_protobuf_codec xnet_net protobuf z)

#add_library(xnet_protobuf_codec_cpp11 ProtobufCodecLite.cc)
#set_target_properties(xnet_protobuf_codec_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x -Wno-error=shadow")
#target_link_libraries(xnet_protobuf_codec_cpp11 xnet_net_cpp11 protobuf z)


install(TARGETS xnet_protobuf_codec DESTINATION lib)
#install(TARGETS xnet_protobuf_codec_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/xnet/net/protobuf)

