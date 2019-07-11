#ifndef CHANGESET_H
#define CHANGESET_H

#ifndef nullptr
#define nullptr 0
#endif // !nullptr
#ifndef INT_MAX
#define INT_MAX -1
#endif // !INT_MAX
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
	void extendNumItems(); //extend max value buildable
	void maxNumItems(char *, int const, int const &);
	//push new amounts to arrays bills or coins
	void unionSet(char *, int const, int const &); // char: type coin or bill | int const: first index of array, value we're trying to build, int const &: value we're trying to push
	int pop();
	void clear();
	int minNumItems(int const &);
	doubleHash<int> *dh;
	const int *allCoinVals;
	
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
	int *numItems;
	int count; //will update in order to know which row has the max count so that if a row has the maxArraySize amount of vals in array, will update array
	int coinMaxBuild = 0;
	int billMaxBuild = 0;
	int maxArraySize;
	int biggestSum = 0;

	//need to do hash table for number of coins available and number of bills available and decrement each time by one whenever we use a coin/bill
};

changeSet::changeSet(int n) {
	count = 0;
	coins = new int*[n];
	// creating the matrix with zeroes everywhere and incrementing accordingly
	for (int i = 0; i < n; i++) {
		coins[i] = new int[n];
		for (int j = 0; j < n; j++)
			coins[i][j] = INT_MAX;
	}
	//doing the same for the bills
	bills = new int*[n];
	for (int i = 0; i < n; i++) {
		bills[i] = new int[n];
		for (int j = 0; j < n; j++) {
			bills[i][j] = INT_MAX;
		}
	}
	//array which states the min num of items needed to build value
	biggestSum = billMaxBuild * 100 + coinMaxBuild * 100;
	numItems = new int[biggestSum];
	numItems[0] = 0;
	for (int i = 1; i < biggestSum; i++) {
		numItems[i] = INT_MAX;
	}
	maxArraySize = n;
	billMaxBuild = n;
	coinMaxBuild = n;
	dh = new doubleHash<int>();
	allCoinVals = dh->valArray();
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
	delete[] numItems;
	numItems = nullptr;
	biggestSum = 0;
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

void changeSet::extendNumItems()
{
	int *temp = new int[biggestSum];
	temp = numItems;
	delete[] numItems;
	numItems = new int[2 * biggestSum];
	for (int i = 0; i < biggestSum; i++) {
		numItems[i] = temp[i];
	}
	biggestSum *= 2;
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

void changeSet::unionSet(char *type, int const valBuild , int const &currVal) {
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
	for (int i = 0; i <= count; i++) { // less than count since max coins that a value has so far is count so automatically for all values, at count, the value is 0
		if (type == "coin" && coins[valBuild][i] == 0) {
			coins[valBuild][i] = currVal;
		} 	else if (type == "bill") {
			bills[valBuild][i] = currVal;
		}
	}
}
//if ever needed
int changeSet::pop() {
	return 0;
}

void changeSet::clear()
{
	for (int i = 0; i < maxArraySize; i++) {
		for (int j = 0; j < coinMaxBuild; j++) {
			coins[i][j] = 0;
		}
		for (int j = 0; j < billMaxBuild; j++) {
			bills[i][j] = 0;
		}
	}
}

int changeSet::minNumItems(int const &newValCalc)
{
	// If array isn't big enough to calculate this value, then extend it
	if (newValCalc > biggestSum) {
		extendNumItems();
		//call function again and then return
		int returnVal = minNumItems(newValCalc);
		return returnVal;
	}
	//if the number of coins needed for newValCalc is known then return it
	if (numItems[newValCalc] != INT_MAX) {
		return numItems[newValCalc];
	}
	int temp;
	int min = INT_MAX;
	// go through each available coins
	for (int i = 0; i < dh->arraySizes(); i++) {
		// only verify if the value of the currency is smaller than the value we're trying to build
		/*
			TO DO, CREATE A NEW OBJECT FROM DOUBLEHASH AND MAKE IT BE THE ARRAY WHERE COINS ARE STORED
			SINCE IF NOT< A LOT OF EMPTY SPOTS IN ARRAY AS VALUES GO UP BY 5 CENTS EVERYTIME
		*/
		if (allCoinVals[i] < newValCalc) {
			temp = 1 + minNumItems(newValCalc - allCoinVals[i]);
			if (temp < min) {
				min = temp;
			}
		}
	}

	return min;
}

#endif // !CHANGESET_H
