#ifndef HAVEN_THREAD
# define HAVEN_THREAD

#include <thread>
#include <mutex>
#include <atomic>
#include <cassert>
#include <queue>

#define SLEEP_TIME 20

typedef enum {
	idle = 1,
	working = 2,
	waiting = 3,
	dead = 4,
} thread_status_e;

typedef struct s_thread_callback {
	void *datain;
	int in_size;
	void *dataout;
	int out_size;
	std::mutex *mutex;
	int (*fun)(void *in, void*out, int, int);
} t_thread_callback;

typedef struct s_thread_pool {
	std::thread sync;
	std::thread *thread;
	std::atomic_int *th_status;
	std::mutex **out;
	size_t max_thread;
} t_thread_pool;

static t_thread_pool *thread_pool;
static std::queue <t_thread_callback> callback;
static std::mutex queue_mtx;

static t_thread_callback createCallback(void *in, void *out, std::mutex *mtx, int in_size, int out_size, int (*fun)(void*, void*, int, int)) {
	t_thread_callback callback;
	callback.in_size = in_size;
	callback.out_size = out_size;
	callback.fun = fun;
	callback.mutex = mtx;
	callback.datain = in;
	callback.dataout = out;
	return (callback);
}

static void addCallbackFunction(t_thread_callback new_callback) {
	for (;;) {
		if (queue_mtx.try_lock()) {
			callback.push(new_callback);
			queue_mtx.unlock();
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}
	assert(false);
}

static void syncThreadPool(t_thread_pool *thread_pool, std::atomic_bool *join) {
	while (!join->load()) {
		if (queue_mtx.try_lock()) {
			if (callback.size()) {
				for(int i = 0; i < thread_pool->max_thread; i++) {
					if (thread_pool->th_status[i].load() == idle) {
						thread_pool->out[i] = callback.front().mutex;
						thread_pool->out[i]->lock();
						thread_pool->thread[i] = std::thread(callback.front().fun, callback.front().datain, callback.front().dataout, callback.front().in_size, callback.front().out_size);
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
				thread_pool->out[i]->unlock();
				thread_pool->th_status[i].store(idle);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}
}

static void startThreadPool(std::atomic_bool *join) {
	thread_pool = new t_thread_pool;
	assert(thread_pool);

	thread_pool->max_thread = thread_pool->sync.hardware_concurrency();
	thread_pool->max_thread -= (thread_pool->max_thread % 2);
	thread_pool->max_thread *= 0.5;
	thread_pool->thread = new std::thread [thread_pool->max_thread];
	assert(thread_pool->thread);
	thread_pool->th_status = new std::atomic_int [thread_pool->max_thread];
	assert(thread_pool->th_status);
	thread_pool->out = new std::mutex *[thread_pool->max_thread];
	thread_pool->sync = std::thread(syncThreadPool, thread_pool, join);
}

static void endThreadPool(std::atomic_bool *join) {
	join->store(true);
	while (!thread_pool->sync.joinable()) std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	thread_pool->sync.join();
	delete [] thread_pool->thread;
	delete [] thread_pool->th_status;
	delete [] thread_pool->out;
	delete thread_pool;
}

#endif
