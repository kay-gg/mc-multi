#pragma once

#include<gen.hpp>

#include<memory>
#include<unordered_map>
#include<vector>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<functional>



class ChunkHolder {
public:
	ChunkHolder();

	Chunk* getChunk(int chunk_x, int chunk_z);
	void insertChunk(std::unique_ptr<Chunk> chunk);
private:
	std::unordered_map<ChunkCord, std::unique_ptr<Chunk>> chunks;
};

class ThreadPool {
public:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop = false;

	ThreadPool(size_t numThreads);
	~ThreadPool(); // joins all workers

	void enqueue(std::function<void()> task);

private:

    
};

enum GenType {
	FAST,
	SLOW,
};

class World {
public:
	World(int numThreads, std::unique_ptr<ChunkGenerator> generator);
	
	Chunk* getChunk(int x, int z);

private:
	ChunkHolder chunks;
	ThreadPool pool;
	std::unique_ptr<ChunkGenerator> gen;
};