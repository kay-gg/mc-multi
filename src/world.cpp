#include "world.hpp"
#include "chunk.hpp"
#include "gen.hpp"

using namespace std;


World::World(int numThreads, std::unique_ptr<ChunkGenerator> generator) : pool(numThreads), gen(std::move(generator)) {
	printf("World created\n");
}

void World::genChunk(ChunkCord cord) {
	Chunk* existing = chunks.getChunk(cord);
	if (existing) return;

	auto chunk = std::make_unique<Chunk>();
	chunk->state = ChunkState::GENERATING;
	Chunk* ptr = chunk.get();
	chunks.insertChunk(cord, std::move(chunk));

	pool.enqueue([this, ptr]() {
		gen->generate(*ptr);
		ptr->state = ChunkState::DONE;
	});
	
}

ChunkHolder::ChunkHolder() {

}

Chunk* ChunkHolder::getChunk(ChunkCord cord) {
	return chunks.at(cord).get();
}

void ChunkHolder::insertChunk(ChunkCord cord,  unique_ptr<Chunk> chunk) {
	chunks.insert({cord, std::move(chunk)});
}
ThreadPool::ThreadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; i++) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    // sleep until theres a task or stop
                    cv.wait(lock, [this]() {
                        return stop || !taskQueue.empty();
                    });

                    if (stop && taskQueue.empty()) return;

                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                }
                task(); 
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    cv.notify_all(); 
    for (auto& worker : workers) {
        worker.join(); 
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(std::move(task));
    }
    cv.notify_one();
}