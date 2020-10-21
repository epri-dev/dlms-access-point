set(DLMS_LIBRARY_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../DLMS-COSEM")
# we want to use this particular version of the asio library
set(ASIO_ROOT ${DLMS_LIBRARY_BASE_DIR}/lib/asio-1.10.6)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Asio REQUIRED)
find_package(Threads REQUIRED)

# specifics for asio
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_CHRONO)

include_directories(core server websocket ${DLMS_LIBRARY_BASE_DIR}/lib/DLMS-COSEM/include/ ${ASIO_INCLUDE_DIR})

# Create the libraries
add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(websocket)

# Create the documentation 
add_subdirectory(doc)

# Create the docker configuration files
add_subdirectory(docker)

link_directories(${DLMS_LIBRARY_BASE_DIR}/build/lib/DLMS-COSEM/)

## finally some executables
add_executable(DLMS_sim Linux.cpp)
add_executable(Metersim Metersim.cpp)
add_executable(APsim APsim.cpp)
add_executable(HESsim HESsim.cpp)

## and the various required libraries
target_link_libraries(DLMS_sim server core DLMS-COSEM Threads::Threads)
target_link_libraries(Metersim server core DLMS-COSEM Threads::Threads)
target_link_libraries(APsim server core DLMS-COSEM Threads::Threads)
target_link_libraries(HESsim core HESConfig DLMS-COSEM Threads::Threads)

add_dependencies(shared_container Metersim APsim HESsim pdf)

## install things
INSTALL(TARGETS Metersim APsim HESsim DESTINATION bin)
