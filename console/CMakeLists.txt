file(GLOB console_headers *.h)
file(GLOB console_sources *.cpp)

add_executable(console ${console_headers} ${console_sources})

