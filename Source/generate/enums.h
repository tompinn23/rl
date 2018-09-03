#pragma once

enum class Tile {
	WATER,
	DIRT,
	SAND,
	ROCK,
	WOODEN_FLOOR,
	WOODEN_WALL,
};

enum class RoomFlags {
	Wooden_Wall,
	Wooden_Floor,
	Stone_Wall,
	Stone_Floor
};

enum class Biome {
	OCEAN,
	MOUNTAINS,
	GRASSLAND,
	DESERT,
	BEACH,
	RIVER,
};