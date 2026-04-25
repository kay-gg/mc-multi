#include "world.hpp"
#include "chunk.hpp"
#include "gen.hpp"

using namespace std;

// TODO
World::World(int numThreads, GenType genType) : pool(numThreads) {
	
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