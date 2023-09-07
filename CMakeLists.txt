cmake_minimum_required(VERSION 3.10)
project(BSPTree)

# USO:
# 'make compile_test' para compilar solo el ejecutable de testeo.
# 'make compile_game' para compilar solo el ejecutable del juego.
# 'make run_test'     para ejecutar el testeo.
# 'make run_game'     para ejecutar el juego.
# 'make test'         para compilar y ejecutar el modo de testeo.
# 'make game'         para compilar y ejecutar el modo de juego.

# Establecer el estándar C++ a utilizar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Encontrar la librería SFML
#set(SFML_DIR "C:/SFML-2.6.0-windows-vc17-64-bit/SFML-2.6.0/lib/cmake/SFML")
#find_package(SFML 2.6.0 COMPONENTS graphics REQUIRED)

# Añadir archivos para el modo de testeo
set(TEST_SOURCE_FILES
    BSPTree.cpp
    Line.cpp
    main.cpp
)

# Añadir archivos para el modo de juego
#set(GAME_SOURCE_FILES
#    Ball.cpp
#    BSPTree.cpp
#    GameInterface.cpp
#    Line.cpp
#    Maze.cpp
#    game.cpp
#)

# Crear el ejecutable para el modo de testeo
add_executable(bsp_tree_test ${TEST_SOURCE_FILES})
target_include_directories(bsp_tree_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Crear el ejecutable para el modo de juego
#add_executable(bsp_tree_game ${GAME_SOURCE_FILES})
#target_include_directories(bsp_tree_game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(bsp_tree_game sfml-graphics)

# Modos de ejecución
add_custom_target(compile_test
    DEPENDS bsp_tree_test
)

#add_custom_target(compile_game
#    DEPENDS bsp_tree_game
#)

add_custom_target(run_test
    COMMAND bsp_tree_test
    DEPENDS bsp_tree_test
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
#
#add_custom_target(run_game
#    COMMAND bsp_tree_game
#    DEPENDS bsp_tree_game
#    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
#)

add_custom_target(test
    COMMAND bsp_tree_test
    DEPENDS bsp_tree_test
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
#
#add_custom_target(game
#    COMMAND bsp_tree_game
#    DEPENDS bsp_tree_game
#    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
#)
