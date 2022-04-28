#include <iostream>
#include<string>
using namespace std;

class candybag
{
public:
	candybag(size_t num): _num(num), _candy(new string[num]) {};

	void fill(size_t index, const string treat)
	{
		string newTreat = treat;
		_candy[index] = newTreat;
	};

	string eat(size_t index)
	{
		string temp = _candy[index];
		_candy[index]= "";
		return temp;
	};
private:
	size_t _num;
	string * _candy;
};
