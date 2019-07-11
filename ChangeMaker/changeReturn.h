#ifndef CHANGERETURN_H
#define CHANGERETURN_H
#ifndef INT_MAX
#define INT_MAX -1
#endif // !INT_MAX

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
	void updateMoney(int const &, int const &); // same as above for each of the paramaters
	int numValue(int const &); // count the amount of bills/coins are left for given value
	int calculateChange(int const &, int const &); //calculate the change to return depending on total amount given and the bill
	void clearCash();
	int statMinNum(int const &); // probability to see which minimum number will be use if starting to run out of certain bill or coin

private:
	int count; //total number of bills and coins
	int sum; //total sum of mnoney
	int *valMoney;
	const int numMoney = 10;
	const int minAmount = 5; // once a number of coin/bill reaches this value, the chances of it being taken for return diminishes
	doubleHash<int>* dh;
	changeSet* cs;
};

changeReturn::changeReturn() {
	cs = new changeSet(numMoney);
	dh = new doubleHash<int>();
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
}

changeReturn::~changeReturn() {
	count = 0;
	sum = 0;
	delete[] valMoney;
	delete cs;
	delete dh;
}

int changeReturn::num() {
	int temp = 0;
	for (int i = 0; i < numMoney; i++) {
		temp = valMoney[i] / 100;
		if (temp == 0) {
			std::cout << valMoney[i] << " cents: " << dh->numItems(valMoney[i]) << std::endl;
		}
		else {
			std::cout << valMoney[i] << " dollars: " << dh->numItems(valMoney[i]) << std::endl;
		}
	}
	return 0;
}

void changeReturn::updateMoney(int const &value, int const &amount) {
	//first updating total amount
	//NOTE amount can be negative or positive. EG adding 1x $5 will be +1, removing 1 x $5 will be -1
	sum += amount * dh->numItems(value);
	dh->updateMoney(value, amount);
}

int changeReturn::numValue(int const &value) {
	return dh->numItems(value);
}

int changeReturn::calculateChange(int const &total, int const &bill) { // totl is amount that must be paid | bill is the total amount given to be paid
	if (total == 0) {
		return 0.0f;
	}
	if (bill < total) {
		std::cout << "Error, amount due is bigger than amount being paid" << std::endl;
	}
	int change = bill - total;
	int changeCoin = change % 100;
	int changeBill = change / 100;
	std::cout << "Change due is " << changeBill << " dollars and " << changeCoin << " cents" << std::endl;
	
	return 0;
}
void changeReturn::clearCash()
{
	sum = 0;
	count = 0;
	dh->clear();
	cs->clear();
}
int changeReturn::statMinNum(int const &valTest)
{
	return 0;
}
#endif