#include "world.hpp"
#include "chunk.hpp"
#include "gen.hpp"

using namespace std;


World::World(int numThreads, std::unique_ptr<ChunkGenerator> generator) : pool(numThreads), gen(std::move(generator)) {
}
// TODO
Chunk* World::getChunk(int x, int z) {

}
// TODO
ChunkHolder::ChunkHolder() {
	
}
// TODO
Chunk* ChunkHolder::getChunk(int chunk_x, int chunk_z) {

}
// TODO
void ChunkHolder::insertChunk(unique_ptr<Chunk> chunk) {

}
// TODO
ThreadPool::ThreadPool(size_t numThreads) {

}
// TODO
ThreadPool::~ThreadPool() {

}