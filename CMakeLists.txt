cmake_minimum_required(VERSION 3.1)

project(ld48)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

set ( SRC
	src/main.cpp
	src/Game.cpp
	src/Game.h
	src/Map.h
	src/Map.cpp
	src/Player.h
	src/Player.cpp
	src/Torpedo.cpp
	src/Torpedo.h
	src/Treasure.cpp
	src/Treasure.h
	src/Enemy.cpp
	src/Enemy.h
	src/BackgroundLevel.cpp
	src/BackgroundLevel.h
	src/GameLevel.cpp
	src/GameLevel.h
	src/Gui.cpp
	src/Gui.h
	src/Level.cpp
	src/Level.h
	src/ParticlePack.cpp
	src/ParticlePack.h
	src/StartGui.cpp
	src/StartGui.h
	src/OverGui.cpp
	src/OverGui.h
)

add_executable(ld48 ${SRC})
set_target_properties(ld48 PROPERTIES CXX_STANDARD 17)
target_link_libraries(ld48 sfml-graphics sfml-audio)