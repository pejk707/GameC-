cmake_minimum_required(VERSION 3.20)
project(GameCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and build SFML 3.1.0 automatically — no manual install needed
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.1.0
    GIT_SHALLOW    TRUE
)
set(SFML_BUILD_AUDIO   OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

add_executable(GameCpp
    main.cpp
    Game.cpp
    Hero.cpp
    Monster.cpp
    Map.cpp
    TileSet.cpp
    SpriteSet.cpp
    Chest.cpp
    CombatSystem.cpp
    UI.cpp
)

target_link_libraries(GameCpp PRIVATE sfml-graphics sfml-window sfml-system)

# Copy arial.ttf next to the executable
add_custom_command(TARGET GameCpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/Fonts/arial.ttf"
        "$<TARGET_FILE_DIR:GameCpp>/arial.ttf"
)

# Copy SFML DLLs next to the executable (Windows only)
if(WIN32)
    add_custom_command(TARGET GameCpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            "$<TARGET_FILE_DIR:GameCpp>"
    )
endif()
