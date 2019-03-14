#ifndef CHANGESET_H
#define CHANGESET_H

#ifndef nullptr
#define nullptr 0
#endif // !nullptr

#include <iostream>
#include "doubleHash.h"

class changeSet
{
public:
	changeSet(int);
	~changeSet();

	//functions defined if needed in the future
	int top() const; 
	int size() const;
	bool empty() const;
	int capacity(char *) const; //type coin or bill

	// extend the max value buildable
	void extendMax(char *, int); // type coin or bill | int is max value to build
	void maxNumItems(char *, int const, int const &);
	//push new amounts to arrays bills or coins
	void push(char *, int const, int const &); // char: type coin or bill | int const: first index of array, value we're trying to build, int const &: value we're trying to push
	int pop();	
	
private:
	// 2D array for dynamic programming calculating bills from 1 to value
	int **bills; // following structure
	/*
	   amount: 0 [ 0		  ]
	   amount: 1 | 1		  |
	   amount: 2 | 1, 1		  |
	   amount: 3 |1, 1, 1	  |
	   amount: 4 | 1, 1, 1, 1 |
	   amount: 5 | 5		  |
	   amount: 6 [ 5, 1       ]

	   rest is filled automatically discarded/forgotten
	*/
	//same as above but for coins now
	int **coins;
	int count; //will update in order to know which row has the max count so that if a row has the maxArraySize amount of vals in array, will update array
	int coinMaxBuild = 0;
	int billMaxBuild = 0;
	int maxArraySize;

	//need to do hash table for number of coins available and number of bills available and decrement each time by one whenever we use a coin/bill
};

changeSet::changeSet(int n) {
	count = 0;
	coins = new int*[n];
	// creating the matrix with zeroes everywhere and incrementing accordingly
	for (int i = 0; i < n; i++) {
		coins[i] = new int[n];
		for (int j = 0; j < n; j++)
			coins[i][j] = 0;
	}
	//doing the same for the bills
	bills = new int*[n];
	for (int i = 0; i < n; i++) {
		bills[i] = new int[n];
		for (int j = 0; j < n; j++) {
			bills[i][j] = 0;
		}
	}
	maxArraySize = n;
	billMaxBuild = n;
	coinMaxBuild = n;
}

changeSet::~changeSet() {
	for (int i = 0; i < coinMaxBuild; i++) {
		delete[] coins[i];
		coins[i] = nullptr;
	}
	delete[] coins;
	for (int i = 0; i < billMaxBuild; i++) {
		delete[] bills[i];
		bills[i] = nullptr;
	}
	delete[] bills;
	bills = nullptr;
	count = 0;
}

int changeSet::top() const {
	return 0;
}

int changeSet::size() const {
	return count;
}

bool changeSet::empty() const {
	return (size() == 0);
}

int changeSet::capacity(char *type) const {
	if (type == "coin") {
		return coinMaxBuild;
	}
	else if (type == "bill") {
		return billMaxBuild;
	} else {
		return -1;
	}
}

void changeSet::extendMax(char *type, int newVal)  {
	// creating temp array which will point to either coins or bills, depending on the one we need to update.
	int **temp = new int*[newVal];
	// tempMax will be size of the row of the matrix
	int tempMax;
	int newMax = newVal;
	// if of type coin, then temp points to coins else temp points to bills
	if (type == "coin") {
		temp = coins;
		tempMax = coinMaxBuild;
		coinMaxBuild = newMax;
	}
	else if (type == "bill") {
		temp = bills;
		tempMax = billMaxBuild;
		billMaxBuild = newMax;
	}
	//creating new array with new max value 
	int **newArray = new int*[newVal];
	//copying over the rows from our old array
	for (int i = 0; i < tempMax; i++) {
		newArray[i] = temp[i];
		//deleting the row once done copying
		delete[] temp[i];
		temp[i] = nullptr;
	}
	//delete temp once done copying everything
	delete[] temp;
	temp = nullptr;
	//finish initializing everything else to 0
	for (int i = tempMax; i < newVal; i++) {
		for (int j = 0; j < maxArraySize; j++) {
			newArray[i][j] = 0;
		}
	}
	//make temp point to new array created
	temp = newArray;
}

void changeSet::maxNumItems(char *type , int const valBuild , int const &currVal) {
	// updateing the new max array which is the number of bills or coins used to build amount
	int newMax = 2 * maxArraySize;
	//creating a temp double pointer which will be used to 
	int **temp = new int*[newMax];
	// creating the new array with updated amount of maxArraySize
	int **newArray = new int*[newMax];
	int tempMaxBuild;
	// assigning correct 2D array to temp and correct ...MaxBuild
	if (type == "coin") {
		temp = coins;
		tempMaxBuild = coinMaxBuild;
	}
	else if (type == "bill") {
		temp = bills;
		tempMaxBuild = billMaxBuild;
	}
	//iterate through array, building each sub array
	for (int i = 0; i < tempMaxBuild; i++) {
		newArray[i] = new int[newMax];
		//copying everything from current array to new one of specific row
		for (int j = 0; j < maxArraySize; j++) {
			newArray[i][j] = temp[i][j];
		}
		//deleting row of current array
		delete[] temp[i];
		temp[i] = nullptr;
		//inserting rest of elements as 0
		for (int j = maxArraySize; j < newMax; j++) {
			newArray[i][j] = 0;
		}
	}
	//deleting oldn array and pointing it to new updated one.
	delete[] temp;
	temp = newArray;
	//pushing new vbalue onto array
	temp[valBuild][maxArraySize] = currVal;
	// updating maxArraySize
	maxArraySize = newMax;
	//incrementing count
	count++;
}

void changeSet::push(char *type, int const valBuild , int const &currVal) {
	//maximum amount we can build so far in array depending on type
	int maxType;
	//assigning correct value
	if (type == "coin") {
		maxType = coinMaxBuild;
	}
	else if (type == "bill") {
		maxType = billMaxBuild;
	}
	// if the value we're rtying to build is bigger, then exten array
	if (valBuild >= maxType) {
		extendMax(type, valBuild);
	}
	// case of reaching max array size, need to update size of array
	if (size() >= maxArraySize) {
		maxNumItems(type, valBuild, currVal);
	}
	// value is within bounds of array so just put in correct spot
	else {
		for (int i = 0; i <= count; i++) { // less than count since max coins that a value has so far is count so automatically for all values, at count, the value is 0
			if (type == "coin" && coins[valBuild][i] == 0) {
				coins[valBuild][i] = currVal;
			} 	else if (type == "bill") {
				bills[valBuild][i] = currVal;
			}
		}
	}
}
//if ever needed
int changeSet::pop() {
	return 0;
}

#endif // !CHANGESET_H
