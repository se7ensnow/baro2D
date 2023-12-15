cmake_minimum_required(VERSION 3.16)
project(baro2D LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(baro2D
        src/main.cpp
        src/game.cpp
        src/game.h
        Entities/sub.cpp
        Entities/sub.h
        src/headers.h
        States/state.cpp
        States/state.h
        States/game_state.cpp
        States/game_state.h
        Entities/entity.cpp
        Entities/entity.h
        States/main_menu_state.cpp
        States/main_menu_state.h
        UI/gui.cpp
        UI/gui.h
        Components/movement_component.cpp
        Components/movement_component.h
        Components/animation_component.cpp
        Components/animation_component.h
        Components/hitbox_component.cpp
        Components/hitbox_component.h
        UI/pause_menu.cpp
        UI/pause_menu.h
        States/settings_state.cpp
        States/settings_state.h
        Map/map.cpp
        Map/map.h
        Controls/control.cpp
        Controls/control.h
        UI/sonar.cpp
        UI/sonar.h
        Utils/sf_math.h
        Utils/sf_shapes.h)

target_link_libraries(baro2D PRIVATE sfml-graphics sfml-audio sfml-system sfml-window sfml-network)
target_compile_features(baro2D PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET baro2D
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:baro2D>
        VERBATIM)
endif()

install(TARGETS baro2D)
