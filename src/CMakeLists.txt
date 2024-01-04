cmake_minimum_required(VERSION 3.26)
project(BottleFlip C)

set(CMAKE_C_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

include_directories(./include)
link_directories(./lib)

add_executable(BottleFlip common.h
        main.c main.h
        menu.c menu.h
        game.c game.h
        player.c player.h
        block.c block.h
        utils/audio.c utils/audio.h
        utils/display.c utils/display.h
        utils/input.c utils/input.h
)

target_link_libraries(BottleFlip SDL2.dll SDL2_image.dll SDL2_ttf.dll SDL2_mixer.dll)