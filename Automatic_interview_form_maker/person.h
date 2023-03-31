#pragma once
#include<iostream>
#include<string>
#include<vector>

using namespace std;

class person
{
public:
	person();

	int timeSize;
	int freeSize;
	int mainTime;
	string teamNumber;
	string name;

	bool is_main;
	bool is_arranged;


	vector<bool> timeArr;
};