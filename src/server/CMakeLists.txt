# we want to use this particular version of the asio library
set(ASIO_ROOT ${DLMS_LIBRARY_BASE_DIR}/lib/asio-1.10.6)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Asio REQUIRED)
find_package(Threads REQUIRED)

# specifics for asio
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_CHRONO)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/ ${DLMS_LIBRARY_BASE_DIR}/lib/DLMS-COSEM/include/ ${ASIO_INCLUDE_DIR})

link_directories(${DLMS_LIBRARY_BASE_DIR}/build/lib/DLMS-COSEM/)
set(DLMS_SERVER_COMMON_SOURCES LinuxCOSEMServer.cpp LinuxClock.cpp LinuxData.cpp LinuxDisconnect.cpp LinuxImageTransfer.cpp)

add_library(server ${DLMS_SERVER_COMMON_SOURCES})
