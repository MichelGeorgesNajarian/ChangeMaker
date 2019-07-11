#ifndef DOUBLEHASH_H
#define DOUBLEHASH_H

#include <exception>

//class to keep track of how many of each type of currency there currently is

// value is in cents so 500 cents = 5 dollars
enum MONEY { FIVE_C = 5, TEN_C = 10, TWENTYFIVE_C = 25, ONE_C = 100, TWO_C = 200, FIVE_B = 500, TEN_B = 1000, TWENTY_B = 2000, FIFTY_B = 5000, HUNDRED_B = 10000, EMPTY = -1 };

template<typename T>
class doubleHash
{
public:	
	float total();
	doubleHash();
	~doubleHash();
	void updateMoney(T const &, int const); // type: coin or bill, T is which value we're trying to update amount, int is how much we add or remove
	T numItems(int const) const; //return the number of items of value of argument we have
	bool empty(); // verifying if the array is empty, a
	void clear();
	int size();
	int arraySizes();
	int *valArray();

private:
	float sum;
	int count;
	int array_size = 10; // 5 different bill values: 5, 10, 20, 50, 100 dollars  AND 5 different coin values 5, 10, 25, 100, 200 cents
	T *money_count;
	MONEY *money_val;

	int h1(T const &) const; // first hash function
	int h2(T const &) const; // second hash function
};

template<typename T>
doubleHash<T>::doubleHash()
{
	// initializing total number of coins and bills as 0
	count = 0;
	// creating array
	money_count = new T[array_size];

	//creating the array of values which will hold and say which value each index of above arrays represent
	money_val = new MONEY[array_size];
	//initializing all values in array of values as empty

	for (int i = 0; i < array_size; i++) {
		money_count[i] = 0;
		money_val[i] = EMPTY;
	}
}

template<typename T>
doubleHash<T>::~doubleHash() {
	clear();
	delete[] money_count;
	delete[] money_val;
	sum = 0.0;
	count = 0;
}

template<typename T>
float doubleHash<T>::total() {
	return sum; // returns sum of all coins and bills
}
// adding money  so basically updating num of coins and bills of each value
template<typename T>
void doubleHash<T>::updateMoney(T const &obj, int const amount) {
	int num = h1(obj); // first matched index
	int step = h2(obj); // next jumping value
	
	while (1) {
		try {
			int tempAmount;
			// if index not assigned to anything
			if (money_val[num] == EMPTY) {
				if (amount < 0) {
					throw -1;
				}
				else {
					money_val[num] = static_cast<MONEY>(obj); // ERROR HERE money_val is of type MONEY and obj is of type T
					money_count[num] = amount;
				}
				break;
			} // doing seperatly in order to have better time efficiency and not always execute money_val[num] = obj
		      // this will update the number of coins/bills
			else if (money_val[num] == obj) {
				tempAmount = money_count[num] += amount;
				if (tempAmount < 0) {
					throw -1;
				}
				else {
					money_count[num] += amount;
					break;
				}
			} else {
				num = (num + step) % array_size; // calculating next index to try
			}
		}
		catch (int tempAmount) {
			std::cout << std::endl << "Cannot have a negative number or coins or bills. Please try again" << std::endl;
			return;
		} 
	}
	count += amount; // updates total amount of coins and bills
	float tempObj = round(2 * obj / 100); // to be easier to round by multiplying by 2 and then divind by 100 in order to have cents, we only need to round to the nearest full number
	sum += tempObj / 2.0; // updating total amount of money
}

// returning num of coins or bills of value obj
template<typename T>
T doubleHash<T>::numItems(const int obj) const {
	int num = h1(obj);
	int step = h2(obj);
	for (int i = 0; i < array_size; i++) {
		// if index not assigned to anything
		if (money_val[num] == EMPTY) {
			// nothing assigned int this index so amount of coin/bill is 0
			break;
		} else if (money_val[num] == obj) {
			return money_count[num]; // returning number of coins of value obj
		} else {
			num = (num + step) % array_size; //calculating next index to try
		}
	}
	return 0;
}
//returns true if  there are both 0 coins and bills
template<typename T>
bool doubleHash<T>::empty() {
	return (size() == 0);
}

//clear array so put state of each index in both arrays as EMPTY and update counters and sums
template<typename T>
void doubleHash<T>::clear() {
	for (int i = 0; i < array_size; i++) {
		money_count[i] = 0;
		money_val[i] = EMPTY;
	}
}

//
template<typename T>
int doubleHash<T>::size() {
	return count;
}

template<typename T>
int doubleHash<T>::arraySizes()
{
	return array_size;
}

template<typename T>
int * doubleHash<T>::valArray()
{
	int j = 0;
	static int *arrayMon = new int[array_size];
	for (int i = HUNDRED_B; i != EMPTY; i--)
	{
		MONEY mon = static_cast<MONEY>(i);

		switch (mon)
		{
		case FIVE_C:
			arrayMon[j] = i;
			j++;
			break;
		case TEN_C:
			arrayMon[j] = i;
			j++;
			break;
		case TWENTYFIVE_C:
			arrayMon[j] = i;
			j++;
			break;
		case ONE_C:
			arrayMon[j] = i;
			j++;
			break;
		case TWO_C:
			arrayMon[j] = i;
			j++;
			break;
		case FIVE_B:
			arrayMon[j] = i;
			j++;
			break;
		case TEN_B:
			arrayMon[j] = i;
			j++;
			break;
		case TWENTY_B:
			arrayMon[j] = i;
			j++;
			break;
		case FIFTY_B:
			arrayMon[j] = i;
			j++;
			break;
		case HUNDRED_B:
			arrayMon[j] = i;
			j++;
			break;
		default:
			break;
		}
	}
	return arrayMon;
}

//first hashing function
template<typename T>
int doubleHash<T>::h1(T const &obj) const {
	int num = static_cast<int> (obj);
	num = num % array_size; //getting mod value
	num = (num >= 0) ? num : num + array_size; // adding value of size of array if % value is negative
	return num;
}

// second hashing function
template<typename T>
int doubleHash<T>::h2(T const &obj) const {
	int num = static_cast<int> (obj);
	num = (num / array_size) % array_size;//dividing obj value by num of array
	num = (num >= 0) ? num : num + array_size;//adding size of arraya to value if value is negative
	num = (num % 2 == 0) ? num + 1 : num;//if value is dividible by 2, then add 1 to make it odd
	return num;
}

#endif // !DOUBLEHASH_H
