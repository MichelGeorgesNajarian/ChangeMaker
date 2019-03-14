#ifndef CHANGERETURN_H
#define CHANGERETURN_H

#include <exception>
#include <iostream>
#include "changeSet.h"
#include "doubleHash.h"

class changeReturn
{
public:
	changeReturn();
	~changeReturn();
	int num(); // number of each coin or bill
	
	//adding coins, can be either opening new roll or from clients 
	void updateMoney(int value, int amount); // same as above for each of the paramaters
	int numValue(int value); // count the amount of bills/coins are left for given value
	float calculateChange(float total, float bill); //calculate the change to return depending on total amount given and the bill

private:
	int count; //total number of bills and coins
	int sum; //total sum of mnoney
	int *valMoney;
	const int numMoney = 10;
	const int minAmount = 5; // once a number of coin/bill reaches this value, the chances of it being taken for return diminishes
	doubleHash<int> dh;
};

changeReturn::changeReturn() {
	valMoney = new int[numMoney];
	valMoney[0] = 5;
	valMoney[1] = 10;
	valMoney[2] = 25;
	valMoney[3] = 100;
	valMoney[4] = 200;
	valMoney[5] = 500;
	valMoney[6] = 1000;
	valMoney[7] = 2000;
	valMoney[8] = 5000;
	valMoney[9] = 10000;
	count = 0;
	sum = 0;
	changeSet cs(10);
}

changeReturn::~changeReturn() {
	count = 0;
	sum = 0;
	delete[] valMoney;
}

int changeReturn::num() {
	int temp = 0;
	for (int i = 0; i < numMoney; i++) {
		temp = valMoney[i] / 100;
		if (temp == 0) {
			std::cout << valMoney[i] << " cents: " << dh.numItems(valMoney[i]) << std::endl;
		}
		else {
			std::cout << valMoney[i] << " dollars: " << dh.numItems(valMoney[i]) << std::endl;
		}
	}
	return 0;
}

void changeReturn::updateMoney(int value, int amount) {
	//first updating total amount
	//NOTE amount can be negative or positive. EG adding 1x $5 will be +1, removing 1 x $5 will be -1
	sum += amount * dh.numItems(value);
	dh.updateMoney(value, amount);
}

int changeReturn::numValue(int value) {
	return 0;
}

float changeReturn::calculateChange(float total, float bill) {
	return 0.0f;
}
#endif