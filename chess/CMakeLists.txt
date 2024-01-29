cmake_minimum_required(VERSION 3.5)

# mkdir build && cd build && rm -Rf * && cmake -G Ninja -DRELEASE=ON .. && ninja
# mkdir build && cd build && rm -Rf * && cmake -DRELEASE=OFF .. && make

project(chess)

include_directories("include")

add_executable(chess
	chess.c
	board.c
	pawn.c
	king.c
	knight.c
	brq.c
	move.c
	compare.c
	game.c
	#	tree.c
	mapper.c
	)

# Définir une option pour activer/désactiver les drapeaux de compilation
option(RELEASE "Activer les drapeaux d'optimisation" ON)

# Définir les drapeaux de compilation en fonction de l'option
if (RELEASE)
	set(CMAKE_GENERATOR "Ninja")
	set(CMAKE_C_COMPILER clang)
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Os")
	add_compile_definitions(NO_LOG)
else()
	set(CMAKE_GENERATOR "Unix Makefiles")
	set(CMAKE_BUILD_TYPE Debug)
endif()
