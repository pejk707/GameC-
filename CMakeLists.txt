cmake_minimum_required(VERSION 3.20)
project(GameCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use downloaded SFML binaries
set(SFML_DIR "D:/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(GameCpp
    main.cpp
    Game.cpp
    Hero.cpp
    Monster.cpp
    Map.cpp
    TileSet.cpp
    Chest.cpp
    CombatSystem.cpp
    UI.cpp
)

target_include_directories(GameCpp PRIVATE "D:/SFML-2.6.1/include")
target_link_libraries(GameCpp PRIVATE sfml-graphics sfml-window sfml-system)

# Copy arial.ttf next to the executable after build
add_custom_command(TARGET GameCpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/Fonts/arial.ttf"
        "$<TARGET_FILE_DIR:GameCpp>/arial.ttf"
)

# Copy tiles/ folder if он существует (для пользовательских PNG спрайтов)
if(EXISTS "${CMAKE_SOURCE_DIR}/tiles")
    add_custom_command(TARGET GameCpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/tiles"
            "$<TARGET_FILE_DIR:GameCpp>/tiles"
    )
endif()

# Copy SFML DLLs next to the executable
if(WIN32)
    add_custom_command(TARGET GameCpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "D:/SFML-2.6.1/bin"
            "$<TARGET_FILE_DIR:GameCpp>"
    )
endif()
