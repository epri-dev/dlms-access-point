set(DLMS_LIBRARY_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../DLMS-COSEM")
# we want ot use this particular version of the asio library
set(ASIO_ROOT ${DLMS_LIBRARY_BASE_DIR}/lib/asio-1.10.6)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Asio REQUIRED)
find_package(Threads REQUIRED)

# specifics for asio
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_CHRONO)

include_directories(${CMAKE_CURRENT_LIST_DIR} ${DLMS_LIBRARY_BASE_DIR}/lib/DLMS-COSEM/include/ ${ASIO_INCLUDE_DIR})

# Create the documentation 
add_subdirectory(doc)

# Create the docker configuration files
add_subdirectory(docker)

link_directories(${DLMS_LIBRARY_BASE_DIR}/build/lib/DLMS-COSEM/)
set(DLMS_COMMON_SOURCES LinuxScheduler.cpp LinuxBaseLibrary.cpp LinuxCore.cpp LinuxDebug.cpp LinuxMemory.cpp LinuxSerial.cpp LinuxSimpleTimer.cpp LinuxSocket.cpp LinuxSynchronization.cpp)
set(DLMS_SERVER_COMMON_SOURCES LinuxCOSEMServer.cpp LinuxClock.cpp LinuxData.cpp LinuxDisconnect.cpp LinuxImageTransfer.cpp)

## finally some executables
add_executable(DLMS_sim Linux.cpp ${DLMS_SERVER_COMMON_SOURCES} ${DLMS_COMMON_SOURCES})
add_executable(Metersim Metersim.cpp ${DLMS_SERVER_COMMON_SOURCES} ${DLMS_COMMON_SOURCES})
add_executable(HESsim HESsim.cpp ${DLMS_COMMON_SOURCES})

## and the various required libraries
target_link_libraries(DLMS_sim DLMS-COSEM Threads::Threads)
target_link_libraries(Metersim DLMS-COSEM Threads::Threads)
target_link_libraries(HESsim DLMS-COSEM Threads::Threads)

## install things
INSTALL(TARGETS Metersim HESsim DESTINATION bin)
