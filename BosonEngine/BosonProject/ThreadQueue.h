#include <iostream>
#include <string>
#include "Debug.h"
#include <cstdint>

using std::string;

using uint64 = uint64_t; //allows us to refrence uint64_t as uint64

///Summary: This is the task struct that all systems will use to send task to be proccess by
///multi threading system. Task structs consist of a generic function pointer that contains process to be run, 
///a pointer to proccess input parameter and pointer to output return for proccess. Each Task 
///must also contain a string pointer containing the name and description for proccess in task. task have a id that
///will be assigned by thread manager.
struct Task
{
	void(*taskFunc)(void*, void*); //generic function pointer to process
	void* input; // pointer to wrapped parameter input.
	void* output; // pointer to wrapped output return
	string* nameDescriptPtr; //pointer to a string containing name and description for task
	uint64 id; //pointer to a int that will hold the unique id for the task as given by thread manager

	//Equal comparator operator for tasks. Compares function pointer, input, output and name
	//Description pointers
	bool operator==(Task taskB)
	{
		if (taskFunc != taskB.taskFunc)
		{
			return false;
		}
		if (input != taskB.input)
		{
			return false;
		}
		if (output != taskB.output)
		{
			return false;
		}
		if (nameDescriptPtr != taskB.nameDescriptPtr)
		{
			return false;
		}
	}

	//Not Equal comparator operator for tasks. Compares function pointer, input, output and name
	//Description pointers
	bool operator!=(Task taskB) {
		if (taskFunc != taskB.taskFunc)
		{
			return true;
		}
		if (input != taskB.input)
		{
			return true;
		}
		if (output != taskB.output)
		{
			return true;
		}
		if (nameDescriptPtr != taskB.nameDescriptPtr)
		{
			return true;
		}
		return false;
	}	
};

class ThreadQueue
{
private:
	Task* queueArray;
	int head;	//keeps track of where the front of the queue is in the array
	int tail;	//keeps track of the back of the queue 
	bool tailBehind = true; //tells whether the back of the queue is in front of the head of the queue in terms of position in array
	int count; //how many items are in queue
	int maxSize; //# of items queue can hold

public:

	//constructors
	ThreadQueue();
	ThreadQueue(int queueSize);

	//destructors
	~ThreadQueue();

	//Core Functions
	/// Summary: The Enqueue Method will properly store a given task into the Thread Queue 
	/// and place it at the end of the queue
	void Enqueue(Task task);

	/// Summary: The Dequeue Method will find next task in the queue and remove the task 
	/// from the queue. The Dequeued task will then be returned.
	Task Dequeue();

	//Utility Functions
	/// Summary: Front gets the task at begining of the queue and returns a copy of it. 
	/// Does not remove task from queue
	Task Front();

	/// Summary: back gets the task at end of the queue and returns a copy of it. Does not 
	/// remove task from queue
	Task Back();

	/// Summary: PrintQueue will go through every spot in array and print out task stored 
	/// there.NOTE: Will Print null Tasks as empty tasks
	void PrintQueue();

	///Summary: self explanatory returns true if queue has no task in it.
	bool IsEmpty();
};
