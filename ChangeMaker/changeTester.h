#ifndef CHANGETESTER_H
#define CHANGETESTER_H

#ifndef nullptr
#define nullptr 0
#endif

//all our custom methods
#include "changeReturn.h"
//infinite loop
#include "runLoop.h"
//mem management class
#include "memManagement.h"

template <class Class_name>
class changeTester
{	protected:
	Class_name *object;
	std::string command;
public:
	changeTester(Class_name *obj = nullptr) :
		object(obj) {
		// emtpy constructor
	}
	int run();
	virtual void process() = 0;
};



#endif // !CHANGETESTER_H

template<class Class_name>
int changeTester<Class_name>::run() {
	memManagement::allocationTtable.stop_recording();
	const static std::string prompt = "%";

	while (true) {
		if (std::cin.eof()) {
			break;
		}

	}

	return 0;
}

template<class Class_name>
void changeTester<Class_name>::process() {

}
