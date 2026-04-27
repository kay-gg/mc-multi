#include "gen.hpp"
#include "omp.h"

void FlatWorldGen::generate(Chunk& chunk) {
	#pragma omp parallel for collapse(3)
	for (int x = 0; x < chunk.SIZE_X; x++) 
	{
		for (int y = 0; y < chunk.SIZE_Y; y++)
		{
			for (int z = 0; z < chunk.SIZE_Z; z++)
			{
				chunk.block_arr[x][y][z] = Block{STONE};
			}
		}
	}
}

void FlatWorldGenSequential::generate(Chunk& chunk) {
	std::cout << "Generating chunk at (" << chunk.cords.x_pos << "," << 0 << "," << chunk.cords.z_pos << ")" << std::endl;

	for (size_t i = 0; i < chunk.SIZE_X; i++)
	for (size_t j = 0; j < chunk.SIZE_Y; j++)
	for (size_t k = 0; k < chunk.SIZE_Z; k++)
	{
		chunk.block_arr[i][j][k] = Block {STONE};
	}
}

void FlatWorldGenDebug::generate(Chunk& chunk) {
	std::cout << "Generating chunk at (" << chunk.cords.x_pos << "," << 0 << "," << chunk.cords.z_pos << ")" << std::endl;
	
	for (size_t i = 0; i < chunk.SIZE_X; i++)
	for (size_t j = 0; j < chunk.SIZE_Y; j++)
	for (size_t k = 0; k < chunk.SIZE_Z; k++)
	{
		chunk.block_arr[i][j][k] = Block {static_cast<BlockType>(k%3)};
	}
		

}