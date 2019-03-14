// ChangeMaker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <exception>
#include "changeTester.h"

class badUsierInput : public std::exception {
	virtual const char* what() const throw() {
		return "Invalid entry, try again.";
	}
} invalidEntry;


int main(int argc, char *argv[]) {

	bool repeat = true;
	const char *billName[] = { "$100", "$50", "$20", "$10", "$5" };
	int bills[] = { 0, 0, 0, 0, 0 };
	const char *coinName[] = { "$2", "$1", "25 cents", "10 cents", "5 cents" };
	int coins[] = { 0, 0, 0, 0, 0 };
	while (repeat) {
		for (int i = 0; i < 5; i++) {
			std::cout << "Please enter number of " << billName[i] << " bills and then press enter: ";
			int temp;
			std::cin >> temp;
			try
			{
				if (!(std::cin >> temp)) {
					throw invalidEntry;
				} else {
					bills[i] = temp;
				}
			}
			catch (std::exception& e)
			{
				std::cout << std::endl << e.what() << std::endl;
				i--;
			}
		}

		std::cout << std::endl << std::endl;

		for (int i = 0; i < 5; i++) {
			std::cout << "You have entered " << bills[i] << " of " << billName[i] << std::endl;
		}

		std::cout << std::endl << "Are there any modifications to make? [Y/N] ? ";
		char mod;
		std::cin >> mod;
		std::cout << std::endl << std::endl;

		switch (mod) {
		case 'Y' || 'y': {
			repeat = true;
			break;
		}
		case 'y': {
			repeat = true;
			break;
		}
		case 'N': {
			repeat = false;
			break;
		}
		case 'n': {
			repeat = false;
			break;
		}
		default: {
			repeat = true;
		}
		}
	}


	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
