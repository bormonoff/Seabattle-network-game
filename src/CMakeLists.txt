add_executable(game
    main.cpp
    core/field.h
    core/field.cpp
    core/network.h
    core/network.cpp
    core/handlegame.h
    core/handlegame.cpp
)

target_link_libraries(game
  Boost::boost
  Threads::Threads
)
