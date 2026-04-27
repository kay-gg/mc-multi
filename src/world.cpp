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

std::vector<Chunk*> World::getAllChunks() {
    return chunks.getChunks(); 
}

void World::debugChunk(ChunkCord cord) {
	Chunk* c = chunks.getChunk(cord);
    if (!c) {
        cout << "Chunk not found at " << cord.x_pos << ", " << cord.z_pos << endl;
        return;
    }

	while (c->state != DONE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    for (int x = 0; x < Chunk::SIZE_X; x++)
    //for (int y = 0; y < Chunk::SIZE_Y; y++)
    for (int z = 0; z < Chunk::SIZE_Z; z++) {
        cout << "(" << x << "," << 0 << "," << z << ") = " 
            << c->block_arr[x][0][z].type << "\n";
    }
}

ChunkHolder::ChunkHolder() {

}

Chunk* ChunkHolder::getChunk(ChunkCord cord) {
    auto it = chunks.find(cord);
    if (it == chunks.end()) {
        return nullptr;
    }
    return it->second.get();
}

std::vector<Chunk*> ChunkHolder::getChunks() {
	std::vector<Chunk*> result;
    for (auto& pair : chunks) {
        result.push_back(pair.second.get());
    }
    return result;
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