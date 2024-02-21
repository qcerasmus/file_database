file(GLOB updater_headers *.h)
file(GLOB updater_sources *.cpp)

add_executable(database_updater ${updater_headers} ${updater_sources})

