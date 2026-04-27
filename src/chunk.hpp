#pragma once

#include "raylib.h"
#include <string>
#include <atomic>

enum BlockType {
	AIR,
	STONE,
	DIRT,
	BEDROCK,
};

enum ChunkState {
	NONE,
	GENERATING,
	DONE,
};

struct Block {
	BlockType type;
};

struct ChunkCord {
	int x_pos;
	int z_pos;

	bool operator==(const ChunkCord& other) const {
        return x_pos == other.x_pos && z_pos == other.z_pos;
    }
};

namespace std {
    template<>
    struct hash<ChunkCord> {
        size_t operator()(const ChunkCord& c) const {
            size_t hx = std::hash<int>{}(c.x_pos);
            size_t hz = std::hash<int>{}(c.z_pos);
            return hx ^ (hz << 32); // combine the two hashes
        }
    };
}

struct Chunk {
	static constexpr int SIZE_X = 16;
	static constexpr int SIZE_Y = 384; // height
	static constexpr int SIZE_Z = 16;

	Block block_arr[SIZE_X][SIZE_Y][SIZE_Z];
	std::atomic<ChunkState> state;
	ChunkCord cords;
};

