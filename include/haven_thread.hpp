#ifndef HAVEN_THREAD
# define HAVEN_THREAD

#include <thread>
#include <mutex>
#include <atomic>
#include <cassert>
#include <queue>

typedef enum {
	idle = 1,
	working = 2,
	waiting = 3,
	dead = 4,
} thread_status_e;

typedef struct s_thread_callback {
	void *datain;
	std::mutex in_mtx;
	std::atomic<void *> dataout;
	int (*fun)(void *in, void*out);
} t_thread_callback;

typedef struct s_thread_pool {
	std::thread sync;
	std::thread *thread;
	std::atomic_int *th_status;
	std::atomic<void *> *out;
	size_t max_thread;
} t_thread_pool;

static t_thread_pool *thread_pool;
static std::queue <t_thread_callback> callback;
static std::mutex queue_mtx;

template <typename T>
static void addCallbackFunction(t_thread_callback new_callback) {
	queue_mtx.lock();
	callback.push(new_callback);
	queue_mtx.unlock();
}

static void syncThreadPool(t_thread_pool *thread_pool) {
	while (1) {
		if (queue_mtx.try_lock()) {
			queue_mtx.lock();
			if (callback.size()) {
				for(int i = 0; i < thread_pool->max_thread; i++) {
					if (thread_pool->th_status[i].load() == idle) {
						thread_pool->thread[i] = std::thread(callback.front().fun, callback.front().datain, callback.front().dataout.load());
						thread_pool->th_status[i].store(working);
						callback.pop();
					}
				}
			}
			queue_mtx.unlock();
		}
		for(int i = 0; i < thread_pool->max_thread; i++) {
			if (thread_pool->thread[i].joinable()) {
				thread_pool->thread[i].join();
				thread_pool->th_status[i].store(idle);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

static void startThreadPool() {
	thread_pool = new t_thread_pool;
	assert(thread_pool);

	thread_pool->max_thread = thread_pool->sync.hardware_concurrency();
	thread_pool->max_thread -= (thread_pool->max_thread % 2);
	thread_pool->max_thread *= 0.5;
	thread_pool->thread = new std::thread [thread_pool->max_thread];
	assert(thread_pool->thread);
	thread_pool->th_status = new std::atomic_int [thread_pool->max_thread];
	assert(thread_pool->th_status);
	thread_pool->sync = std::thread(syncThreadPool, thread_pool);
}

static void endThreadPool(void) {
	while (!thread_pool->sync.joinable()) std::this_thread::sleep_for(std::chrono::milliseconds(250));
	thread_pool->sync.join();
	delete [] thread_pool->thread;
	delete [] thread_pool->th_status;
	delete thread_pool;
}

#endif
