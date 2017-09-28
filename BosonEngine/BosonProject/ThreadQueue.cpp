#include "ThreadQueue.h"

//Generic non Parameterized constructor for queue.
//DefaultSize: 100 items
ThreadQueue::ThreadQueue()
{
	queueArray = new Task[100];
	head = 0;
	tail = 0;
	count = 0;
	maxSize = 100;
	Debug::Log("Queue Intizialized");
}

///Pramaterized Constructor that takes a int equal to desired size of queue
ThreadQueue::ThreadQueue(int queueSize)
{
	//check to make sure Queue size is a valid size
	if (queueSize <= 0) {
		Debug::Log("Cannot initizalize Queue with size of 0 or less", LogType::Error);
		return;
	}

	queueArray = new Task[queueSize]; //creates the queue Array based on desired size.

	//loop through array an make sure each spot is set to a null Task().
	for (int i = 0; i < queueSize; i++)
	{
		queueArray[i] = Task();
	}

	//Sets up Queue Management Variables
	head = 0;
	tail = 0;
	count = 0;
	maxSize = queueSize;

	Debug::Log("Queue Intizialized");
}

//Destructor for Thread Queue
ThreadQueue::~ThreadQueue()
{
	//deletes all items in queueArray and queueArray itself from memory
	delete[] queueArray;
}

/// Summary: The Enqueue Method will properly store a given task into the Thread Queue 
/// and place it at the end of the queue
void ThreadQueue::Enqueue(Task task)
{
	//Keeps trying to enqueue task until it is able to enqueue it.
	while (true)
	{
		//check to see if there is room in queue
		if (count >= maxSize)
		{
			//tell us that the queue has no room
			Debug::Log("Out of room in queue", LogType::Error);
			continue; //skips past rest of code and goes through next iteration of loop
		}

		//Special Case: The Entire Queue is empty and this will be the first Task stored
		if (count == 0) //checks to see if there are no items in queue
		{
			//make sure tail and head are properly set
			head = 0; //sets head to 0 since that is first position in queueArray
			tail = 0; //tail is set to position of first and only Task so far in queue

			queueArray[head] = task; //stores task in first position in queue
			count++; //increase count by 1

			//Debug Statement to tell what task has been stored and where.
			Debug::Log(*(task.nameDescriptPtr) + " stored at position " + std::to_string(tail) + " in queue");
			return; //exit loop and end method
		}

		// Special Case: There is room in queue, but tail is already at the back of queue array
		// and needs to be move to front of array. (Revolving Queue feature)
		if (tail + 1 >= maxSize) //if next tail position will be past end of queueArray.
		{
			//starts tail at the begining of queue
			if (queueArray[0] == Task()) //Checks to see if first position in array is in fact empty
			{
				tail = 0; //sets tail to be at 0 of array.
				queueArray[0] = task; //stores task at first position in array
				count++; //increase count by 1
				tailBehind = true;

				//DEBUG STATEMENT to toe whats been stored and where
				Debug::Log(*(task.nameDescriptPtr) + " stored at position " + std::to_string(tail) + " in queue");
				return; //exit loop and end method
			}
			else
			{
				Debug::Log("Enqueue case thought there was space in queue when there was not", LogType::Error);
				return;
			}
		}

		//Normal Method for enqueuing Tasks in thread queue
		tail = tail + 1; //moves tail marker over one spot in the array.
		queueArray[tail] = task; //stores task at tail position
		count++; 

		//DEBUG STATEMENT to toe whats been stored and where
		Debug::Log(*(task.nameDescriptPtr) + " stored at position " + std::to_string(tail) + " in queue");
		return;
	}
}

///Summary: The Dequeue Method will find next task in the queue and remove the task from the queue. The Dequeued task will then be returned.
Task ThreadQueue::Dequeue()
{
	//check to see if queue is empty
	if (count <= 0) 
	{
		//Print warning that queue is empty and return a null Task.
		Debug::Log("QUEUE IS EMPTY IMPOSSIBLE TO DEQUEUE", LogType::Warning);
		return Task();
	}

	Task dequeueItem = queueArray[head]; //copy task into local task variable to be returned later
	queueArray[head] = Task(); //Reset position of dequeued task to be set to null
	count--; //decrease items currently in queue by 1

	//Special Case: that was last Task in queue and queue is now empty.
	if (count <= 0) //if there are no items in queue
	{
		//reset all queue management variables
		head = 0;
		tail = 0;
		count = 0;
		return dequeueItem; //return the dequeued Task
	}
	//Special Case:dequed Task was at last position in queueArray and the queue is not empty move head to begin of array
	else if (head + 1 >= maxSize  && count != 0) 
	{
		head = 0; //sets head to begining of array
		tailBehind = false;
		return dequeueItem;
	}
	else 
	{
		head++; //sets new head to be next task in queue
	}

	return dequeueItem;
}

///Summary: Front gets the task at begining of the queue and returns a copy of it. Does not remove task from queue
Task ThreadQueue::Front()
{
	return queueArray[head];
}

///Summary: back gets the task at end of the queue and returns a copy of it. Does not remove task from queue
Task ThreadQueue::Back()
{
	return queueArray[tail];
}

///Summary: PrintQueue will go through every spot in array and print out task stored there.NOTE: Will Print null Tasks as empty tasks
void ThreadQueue::PrintQueue()
{
	Debug::Log("PRINTING QUEUE START"); //used to tell in console when print methoded started
	//loops through entire queue array
	for (int i = 0; i < maxSize; i++)
	{
		if (queueArray[i] != Task()) //check to see if there is task in current spot
		{
			//print out task at that position
			uint64 id = queueArray[i].id;
			string name = *(queueArray[i].nameDescriptPtr);
			Debug::Log("Task " + std::to_string(id) + ": " + name + "| Stored in position " + std::to_string(i) + " in array");
		}
		else
		{
			//print out that there is a empty spot in current position
			Debug::Log("Empty spot at position " + std::to_string(i) + " in array");
		}
	}
	Debug::Log("Printing queue END");
}

///Summary: self explanatory returns true if queue has no task in it.
bool ThreadQueue::IsEmpty()
{
	if (count <= 0)
	{
		return true;
	}
	return false;
}
