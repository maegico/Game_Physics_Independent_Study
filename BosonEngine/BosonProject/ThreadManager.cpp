#include "ThreadManager.h"

ThreadManager* ThreadManager::tmRef = nullptr;	// A static thread manager pointer

// Creates the queues and creates/starts the threads in the thread pool
ThreadManager::ThreadManager()
{
	lowPriorityQueue = new ThreadQueue(200);
	highPriorityQueue = new ThreadQueue(300);

	bStop = false;
#ifndef SINGLE_THREADED
	for(int i = 0; i < 7; i++)
	{
		threadPool[i] = thread(&ThreadManager::run, this);
	}
#endif // SINGLE_THREADED
}

// Doesn't do anything, should probably remove
// The Delete() function takes its place
ThreadManager::~ThreadManager()
{
}

// The function that all the threads will use
// Continuously dequeues a task, then tries to run it until bStop is set to true
void ThreadManager::run()
{
	bool pause = bStop;
	while (!pause)
	{
		mtx.lock();
		Task task = Request();
		mtx.unlock();
		if (task != Task())
		{
			Debug::Log("Running Function: " + std::to_string(task.id));
			task.taskFunc(task.input, task.output);
		}
		pause = bStop;
	}
}

// A static function that returns an instance to a private ThreadManager
// @return: A pointer to ThreadManager
ThreadManager* ThreadManager::GetThreadManager()
{
	if (!tmRef) 
	{
		tmRef = new ThreadManager();
	}

	return tmRef;
}

// Adds the task to one of the two queues
// @param: The task to add to a queue
// @param: Whether to add to the high priority queue or the low priority
void ThreadManager::Add(Task task, bool isHighPriority)
{
//Preproccessor to  run thread system in single thread mode.
#ifdef SINGLE_THREADED
	if (task != Task())
	{
		//give task a unique identifier
		task.id = iCount;

		Debug::Log("Running Function: " + std::to_string(task.id));
		task.taskFunc(task.input, task.output);
	}

	iCount++;
	return;
#endif // SINGLE_THREADED

	//check to see if task is invalid(NULL)
	if (task == Task())
	{
		Debug::Log("Task is invalid(NULL), it was not added", LogType::Error);
		return;
	}

	//give task a unique identifier
	task.id = iCount;

	//check to see whether to add the task to high or low Priority Queues
	if (isHighPriority)
	{
		highPriorityQueue->Enqueue(task);
		//highPriorityQueue->PrintQueue();
	}
	else
	{
		lowPriorityQueue->Enqueue(task);
		//lowPriorityQueue->PrintQueue();
	}

	iCount++;
}

// How the threads grab a task from the queues.
// We check the high priority queue and then the low priority queue.
Task ThreadManager::Request()
{
	//check to see if high Priority Queue is not empty
	Task dequeueTask = Task();
	if (!highPriorityQueue->IsEmpty())
	{
		dequeueTask = highPriorityQueue->Dequeue();
	}
	else if(!lowPriorityQueue->IsEmpty())
	{
		lowPriorityQueue->PrintQueue();
		dequeueTask = lowPriorityQueue->Dequeue();
		lowPriorityQueue->PrintQueue();
	}
	else
	{
		Debug::Log("Nothing in either queue.", LogType::Warning);
		return Task();
	}

	//double check to make sure dequeueTask is not null
	if (dequeueTask != Task())
	{
		return dequeueTask;
	}
	else
	{
		Debug::Log("Dequeued Item was NULL", LogType::Error);
		return Task();
	}
}

// Stops the threads and deletes the queues and the ThreadManager
void ThreadManager::Delete()
{
	bStop = true;
#ifndef SINGLE_THREADED
	for (int i = 0; i < 7; i++)
	{
		//This isn't safe,
		//since if the thread isn't joinable the thread will never be joined
		if(threadPool[i].joinable())
			threadPool[i].join();
	}
#endif // !SINGLE_THREADED

	delete highPriorityQueue;
	delete lowPriorityQueue;

	delete tmRef;
}

// Doesn't actually work and isn't used, ignore it
//void ThreadManager::PauseThreads(bool isPaused)
//{
//	bStop = isPaused;
//}


