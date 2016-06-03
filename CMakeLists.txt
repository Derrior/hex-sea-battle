cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "hex-sea-battle" )

## Set our project name
project(${PROJECT_NAME})


include_directories("src/include")
## Use all the *.cpp files we found under this folder for the project

set (SRC_LIST src/main.cpp
		      src/button.cpp
              src/background.cpp
	          src/init_world.cpp
	          src/music_player.cpp
              src/drawing.cpp
              src/engine.cpp
              src/field.cpp
              src/file.cpp
              src/geom.cpp
              src/polygon.cpp
              src/shader.cpp
              src/ship.cpp
              src/SOIL/image_DXT.c
              src/SOIL/image_helper.c
              src/SOIL/SOIL.c
	      src/SOIL/stb_image_aug.c
			  )
set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -pipe -Wall -Wextra -Wshadow")

## Define the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} m glew32 freeglut opengl32 sdl2)
