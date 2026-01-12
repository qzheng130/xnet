set(base_SRCS
  AsyncLogging.cc
  Condition.cc
  CountDownLatch.cc
  CurrentThread.cc
  Date.cc
  Exception.cc
  FileUtil.cc
  LogFile.cc
  Logging.cc
  LogStream.cc
  ProcessInfo.cc
  Timestamp.cc
  Thread.cc
  ThreadPool.cc
  TimeZone.cc
  )

add_library(xnet_base ${base_SRCS})
target_link_libraries(xnet_base pthread rt)

#add_library(xnet_base_cpp11 ${base_SRCS})
#target_link_libraries(xnet_base_cpp11 pthread rt)
#set_target_properties(xnet_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS xnet_base DESTINATION lib)
#install(TARGETS xnet_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/xnet/base)

if(XNET_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()
