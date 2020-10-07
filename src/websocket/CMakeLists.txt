set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

## finally some executables
add_library(HESConfig HESConfig.cpp server.cpp)
if(Boost_FOUND) 
    target_include_directories(HESConfig PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(HESConfig PRIVATE Boost::system ${CMAKE_THREAD_LIBS_INIT})
endif(Boost_FOUND)

