add_subdirectory(utils)
add_subdirectory(graphics)
add_subdirectory(animations)
add_subdirectory(input)
add_subdirectory(components)
add_subdirectory(game)
add_subdirectory(window)

add_executable(chimarrao-platformer Main.cpp)

target_link_libraries(chimarrao-platformer PUBLIC game)