#pragma once

#include <vector>
#include "PerlinNoise.h"
#include "enums.h"

class Map {

private:
	int mWidth;
	int mHeight;
	int mSeed;
	std::vector<std::vector<double>> heightmap;
	std::vector<std::vector<double>> moisturemap;
	PerlinNoise hn; // Heightmap noise
	PerlinNoise mn; // Moisturemap noise

	// Map gen constants;
	const double freq = 0.04 ;
	const double z_val = 0.5;

public:
	std::vector<std::vector<Biome>>	biomemap;
	Map(int width, int height, int seed);
	void generate_heightmap();
	void generate_moisturemap();
	void assign_biomes();
	void walk_rivers();
	std::vector<std::vector<Tile>> tilemap;
};