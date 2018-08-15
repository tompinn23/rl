#include "generate.h"

Map::Map(int width, int height, int seed)
{
	mWidth = width;
	mHeight = height;
	mSeed = seed;
	hn = PerlinNoise(seed);
	mn = PerlinNoise(seed * 2);
	heightmap.resize(width);
	for (auto &a : heightmap)
		a.resize(height);
	moisturemap.resize(width);
	for (auto &a : moisturemap)
		a.resize(height);
	tilemap.resize(width);
	for (auto &a : tilemap)
		a.resize(height);
	biomemap.resize(width);
	for (auto &a : biomemap)
		a.resize(height);

}

void Map::generate_heightmap()
{
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			heightmap[i][j] = hn.noise(freq *(i), freq *(j), z_val) + 0.5 * hn.noise(2 * freq *(i), 2 * freq *(j), z_val);

		}
	}
}

void Map::generate_moisturemap()
{
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			moisturemap[i][j] = mn.noise(freq *(i), freq *(j), z_val);
		}
	}
}

void Map::assign_biomes()
{
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (heightmap[i][j] < 0.5)
				biomemap[i][j] = Biome::OCEAN;
			else if (heightmap[i][j] < 0.6)
			{
				if(moisturemap[i][j] < 0.6)
					biomemap[i][j] = Biome::BEACH;
				else
				{
					biomemap[i][j] = Biome::GRASSLAND;
				}
			}
			else if (heightmap[i][j] < 0.92)
			{
				if (moisturemap[i][j] < 0.4)
					biomemap[i][j] = Biome::DESERT;
				else
					biomemap[i][j] = Biome::GRASSLAND;
			}
			else {
				biomemap[i][j] = Biome::MOUNTAINS;
			}
		}
	}
}

void Map::walk_rivers()
{
}
