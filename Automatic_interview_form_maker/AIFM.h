#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <streambuf>
#include <stdlib.h>
#include<sstream>
#include"person.h"
#include<vector>
#include<ctime>
#include"Size.h"

using namespace std;

class AIFM
{
public:
	void input();
	void run();
	bool swap(person& a, vector<person>& arr);
	bool adaptiveRun(person& a);
	int partition(vector<Size>& arr, int low, int high);
	void quickSort(vector<Size>& arr, int low, int high);
	bool transfer(int index);
	void output();

	void show1();
	void show2();
private:
	int personSize;
	int remainPersonSize;
	int timeSize;
	int model;

	bool is_team;

	vector<string> timeNameArr;
	vector<int> timeArrSizeArr;
	vector<int> timeArrMaxSizeArr;
	vector<Size> availableArr;
	vector<vector<person>> timeArrPtrArr;
	vector<person> sourceArr;
	vector<person> finalArr;
};