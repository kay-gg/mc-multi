#include "gen.hpp"

// TODO
void FlatWorldGen::generate(Chunk& chunk) {

}

// TODO
void FlatWorldGenSequential::generate(Chunk& chunk) {
	for (size_t i = 0; i < chunk.SIZE_X; i++)
	{
		for (size_t j = 0; j < chunk.SIZE_Y; j++)
		{
			for (size_t k = 0; k < chunk.SIZE_Z; k++)
			{
				chunk.block_arr[i][j][k] = Block {STONE};
			}
			
		}
		
	}
	
}