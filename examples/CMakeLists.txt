
add_executable(hello_thread hello_thread.cpp)
add_executable(hello_mutex hello_mutex.cpp)
add_executable(hello_promise hello_promise.cpp)
add_executable(hello_future hello_future.cpp)
add_executable(hello_condition_var hello_condition_variable.cpp)

set(includeDir "${CMAKE_SOURCE_DIR}/include")

add_executable(hello_semaphore hello_semaphore.cpp)
target_include_directories(hello_semaphore PRIVATE "${includeDir}")

add_executable(hello_pool hello_pool.cpp)
target_include_directories(hello_pool PRIVATE "${includeDir}")

add_executable(prod_con prod_con.cpp)
target_include_directories(prod_con PRIVATE "${includeDir}")
