#include<iostream>
#include"Size.h"

using namespace std;

Size::Size()
{
	this->index = 0;
	this->nullNum = 0;
}

Size::Size(int index, int nullNum)
{
	this->index = index;
	this->nullNum = nullNum;
}
