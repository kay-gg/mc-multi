#pragma once

#include "chunk.hpp"
#include<iostream>

class ChunkGenerator {
public:
	virtual void generate(Chunk& chunk) = 0;

	virtual ~ChunkGenerator() = default;
};

class FlatWorldGen : public ChunkGenerator {
public:
	void generate(Chunk& chunk) override;
};

class FlatWorldGenSequential : public ChunkGenerator {
public:
	void generate(Chunk& chunk) override;
};

class FlatWorldGenDebug : public ChunkGenerator {
public:
	void generate(Chunk& chunk) override;
};


// TODO
// Not necessary, just if we get bored
class WorldGen : public ChunkGenerator {

};

class WorldGenSlow : public ChunkGenerator {

};