//#define SINGLE_THREADED //DO NOT REMOVE SWITCHES SYSTEM FROM MULTI TO SINGLE THREADED
#pragma once
#include <thread>
#include <cstdint>
#include "ThreadQueue.h"
#include <mutex>
#include <cstdint>
#include <vector>

using std::vector;
using std::thread;
using std::mutex;
using uint64 = uint64_t;
using uint8 = uint8_t;

class ThreadManager
{
public:
	// A static function that creates a private ThreadManager if one does not exist
	// @return: A pointer to ThreadManager
	static ThreadManager* GetThreadManager();

	// Adds the task to one of the two queues
	// @param: The task to add to a queue
	// @param: Whether to add to the high priority queue or the low priority
	void Add(Task task, bool isHighPriority = false);
	// How the threads grab a task from the queues.
	// We check the high priority queue and then the low priority queue.
	Task Request();
	// Stops the threads and deletes the queues and the ThreadManager
	void Delete();

	// Doesn't actually work and isn't used, ignore it
	//void PauseThreads(bool isPaused);

	int getHdwConcurrency();

private:
	static ThreadManager* tmRef;	// A static thread manager pointer

	bool bStop;						// whether to stop the threads
	uint8 hdwConcurrency = 0;
	uint64 iCount = 0;				// The number of operations run

	mutex mtx;						// Manages the queues
	//thread threadPool[7];			// A basic thread pool that runs the tasks in the queues
	std::vector<thread*> threadPool;

	ThreadQueue* lowPriorityQueue;	//a queue that stores low priority tasks
	ThreadQueue* highPriorityQueue;	//a queue that stores high priority task

	//don't have to deal with a move constructor, since we defined a copy constructor
		//doesn't matter that it is deleted
		//move constructor only declared implicitly if you don't declared a copy constructor
	// Creates the queues and creates/starts the threads in the thread pool
	ThreadManager();											//normal constructor
	ThreadManager(const ThreadManager&) = delete;				//copy constructor
	ThreadManager& operator=(const ThreadManager&) = delete;	//assignment operator
	// Doesn't do anything, should probably remove
	// The Delete() function takes its place
	~ThreadManager();											//destructor

	// The function that all the threads will use
	// Continuously dequeues a task, then tries to run it until bStop is set to true
	void run();
};

