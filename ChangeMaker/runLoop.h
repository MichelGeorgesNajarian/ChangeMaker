#ifndef RUNLOOP_H
#define RUNLOOP_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <string>
#include <sstream>
#include "memManagement.h"

template <class Class_name>
class runLoop
{
protected:
	Class_name *object;
	std::string command;
public:
	runLoop(Class_name *obj = nullptr) :
	object(obj) {
		//empty constructor
	}
	int run();
	virtual void process() = 0;
};

template<class Class_name>
int runLoop<Class_name>::run()
{
	memManagement::allocation_table.stop_recording();

	const static std::string prompt = " % ";

	while(true) {
		if (std::cin.eof()) {
			break;
		}

		++memManagement::count;
		std::cout << memManagement::count << prompt;

		std::cin >> command;

		if (std::cin.eof()) {
			std::cout << "Exiting..." << std::endl;
			break;
		}

		/*If user enters !!,
				then repeat the last command
		  If the user enters !n where n is a number 1 <= n < count
			then set the new command the be the nth command
		*/

		if (command == "!!") {
			if (memManagement::count == 1) {
				std::cout << "Event not found. This is the first command. Please try again." << std::endl;
				continue;
			}
			//	command == memManagement::history[count - 1];
			std::cout << std::endl << "Previous command was " << command;
		}
		else if (command[0] == "!") {
			int n;
			std::istringstream number(command.substr(1, command.length() - 1));
			number >> n;

			if (n <= 0 || n >= memManagement::count || n >= 1000) {
				std::cout << "Event not found" << std::endl;
				continue;
			}

			command = memManagement::history[n];
		}
		//track memory allocations made
		memManagement::allocation_table.start_recording();
		
		//key commands: delete, summary, details, exit and memory
		if (command == "delete") {
			delete object;
			object = nullptr;
			std::cout << "Okay..." << std::endl;
		}
		else if (command == "summary") {
			memManagement::allocation_table.summary();
		} 
		else if (command == "details") {
			memManagement::allocation_table.details();
		}
		else if (command == "exit") {
			memManagement::allocation_table.stopt_recording();
			std::cout << "Okay..." << std::endl;
			break;
		}
		else if (command == "memory") {
					
		}
	}

	return 0;
}

#endif
