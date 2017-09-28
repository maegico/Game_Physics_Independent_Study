#pragma once
#include "ThreadManager.h"

//The header file with all the Tests for the Multithreading system
//There is no corresponding cpp file

namespace ThreadTesting
{
	ThreadManager* tm;
	unsigned long long smallInput = 8;
	unsigned long long largeInput = 5752710;
	string facName = "Small Factorial";
	string fibName = "Large Fibonnacci";

	// Calls the GetThreadManager function and saves the returned
	// instance into a local pointer
	void StartThreadManager()
	{
		tm = ThreadManager::GetThreadManager();
	}
	
	//A function that returns the factorial of a number using recursion
	unsigned int RecursiveFactorial(unsigned int number)
	{
		return number <= 1 ? number : RecursiveFactorial(number - 1)*number;
	}

	//A form of the factorial function above, that doesn't use recursion
	long NonrecursiveFactorial(unsigned int number)
	{
		long fact = 1;

		//for Loop Block
		for (unsigned int counter = 1; counter <= number; counter++)
		{
			fact = fact * counter;
		}

		return fact;
	}

	//A function that returns the fibonacci number of a number passed in
	unsigned long long fibo(long long n) {
		if (n <= 1) {
			return n;
		}
		unsigned long long fibo = 1;
		unsigned long long fiboPrev = 1;
		for (unsigned long long i = 2; i < n; ++i) {
			unsigned long long temp = fibo;
			fibo += fiboPrev;
			fiboPrev = temp;
		}
		return fibo;
	}

	//The basic form of a small operation for the threads to run
	void SystemSmallLoad(void* input, void* output)
	{
		//unsigned int num = NonrecursiveFactorial(*(int*)input);
		unsigned long long num = fibo(*(long long*)input);
		Debug::Log("Factorial Number is: " + std::to_string(num));
	}

	//the basic form of a large operation for the threads to run
	void SystemLargeLoad(void* input, void* output)
	{
		//unsigned int num = NonrecursiveFactorial(*(int*)input);
		unsigned long long num = fibo(*(long long*)input);
		Debug::Log("Factorial Number is: " + std::to_string(num));
	}
	
	//Below is a list of all the various unit tests for the thread system

	//Test Cases:

	//1) Lots of Small Loads
	void smallLoads()
	{
		//string name = "Small Factorial";
		//uint64 ids[100000];
		//int input = 8;
		for (int i = 10000; i; i--)
		{
			
			//ids[i-1] = i;
			Task task = { SystemSmallLoad, &smallInput, nullptr, &facName, 0 };
			tm->Add(task);
			//Debug::Log("I is: " + std::to_string(i));
		}
	}

	//2) Lots of Large Loads
	void largeLoads()
	{
		/*string name = "Large Fibonnacci";
		uint64 ids[5000];*/
		//unsigned long long input = 5752710;
		//long long input = 35752710;
		//long long input = 95752710;
		for (int i = 5000; i; i--)
		{

			//ids[i - 1] = i;
			Task task = { SystemLargeLoad, &largeInput, nullptr, &fibName, 0 };
			tm->Add(task);
			//Debug::Log("I is: " + std::to_string(i));
		}
	}

	//3) Lots of Varying Granularity Loads
		//Essentially, we switch between large and small loads
	void varyingGranularityLoads()
	{
		/*string name = "Large Fibonnacci";
		uint64 ids[100000];
		unsigned long long smallInput = 5752710;
		unsigned long long largeInput = 5752710;*/
		//long long input = 35752710;
		//long long input = 95752710;
		for (int i = 10000; i; i--)
		{
			if (i % 4 == 0)
			{
				Task task = { SystemLargeLoad, &largeInput, nullptr, &fibName, 0 };
				tm->Add(task);
				//Debug::Log("I is: " + std::to_string(i));
			}
			else
			{
				Task task = { SystemSmallLoad, &smallInput, nullptr, &facName, 0 };
				tm->Add(task);
				//Debug::Log("I is: " + std::to_string(i));
			}
		}
	}

	//4) Lots of Varying Loads
		//We switch between large and small loads and putting those loads
		//in either the high priority or low priority queues
	void varyingLoads()
	{
		/*string name = "Large Fibonnacci";
		uint64 ids[100000];
		unsigned long long smallInput = 5752710;
		unsigned long long largeInput = 5752710;*/
		//long long input = 35752710;
		//long long input = 95752710;
		for (int i = 10000; i; i--)
		{
			if (i % 4 == 0)
			{
				Task task = { SystemLargeLoad, &largeInput, nullptr, &fibName, 0 };
				tm->Add(task, (i /4) % 3 == 0);
				//Debug::Log("I is: " + std::to_string(i));
			}
			else
			{
				Task task = { SystemSmallLoad, &smallInput, nullptr, &facName, 0 };
				tm->Add(task, i % 2);
				//Debug::Log("I is: " + std::to_string(i));
			}
		}
	}

	//5) Varying Priority Testing with Small Loads
		//Small loads put into either the high priority or low priority queues
	void varyingPrioritySmallLoads()
	{
		/*string name = "Small Factorial Vary Priority";
		uint64 ids[100000];
		int input = 8;*/
		for (int i = 100000; i; i--)
		{

			//ids[i-1] = i;
			Task task = { SystemSmallLoad, &smallInput, nullptr, &facName, 0 };
			tm->Add(task, (bool)(i%2));
			//Debug::Log("I is: " + std::to_string(i));
		}
	}

	//6) Varying Priority Testing with Large Loads
		//Large loads put into either the high priority or low priority queues
	void varyingPriorityLargeLoads()
	{
		/*string name = "Large Fibonnacci Vary Priority";
		uint64 ids[100000];
		unsigned long long input = 5752710;*/
		//long long input = 35752710;
		//long long input = 95752710;
		for (int i = 100000; i; i--)
		{

			//ids[i-1] = i;
			Task task = { SystemLargeLoad, &largeInput, nullptr, &fibName, 0 };
			tm->Add(task, (bool)(i % 2));
			//Debug::Log("I is: " + std::to_string(i));
		}
	}

	//Never did these two, but we pretty much covered all the bases above,
	//so ignore them
	//7) Varying Priority Testing with Varying Granularities
	//8) Varying Priority Testing with Various Loads
	
	//Calls the thread managers delete function
	void EndThreadManager()
	{
		tm->Delete();
	}
}