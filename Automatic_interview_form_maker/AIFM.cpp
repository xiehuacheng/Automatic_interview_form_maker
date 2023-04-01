#include"AIFM.h"
//使用前请务必检查一下两个define中的内容是否正确
#define FREE "A.可以"
#define NOT_FREE "B.不可以"
using namespace std;

void AIFM::input()
{
	//需求信息的获取
	cout << "请将表格放入文件夹中并输入文件名（后缀名为.csv）：";
	string tempFileName;
	cin >> tempFileName;

	cout << "选择功能：0、面试者；1、面试校队" << endl;
	bool tempTeam;
	cin >> tempTeam;

	cout << "请输入总人数：";
	int tempPersonSize;
	cin >> tempPersonSize;

	cout << "请输入需要安排的时间段数量：";
	int tempTimeSize;
	cin >> tempTimeSize;

	//基本数据的初始化
	this->personSize = tempPersonSize;
	this->timeSize = tempTimeSize;
	this->is_team = tempTeam;

	//根据用户的输入选择正确的运行模式
	cout << "请选择运行模式：0、普通模式；1、自适应模式（根据给定的范围进行选择运行）" << endl;
	int tempModel;
	cin >> tempModel;

	this->model = tempModel;

	if (this->model == 0)
	{
		//数组的构建和获取各面试场次的人数
		for (int i = 0; i < tempTimeSize; i++)
		{
			cout << "请输入第" << i + 1 << "个时间段的名称<与表中首选面试时间的文字保持一致>（如：周六上午）：" << endl;
			string tempName;
			cin >> tempName;
			cout << "请输入" << tempName << "可以安排的数量：" << endl;
			int tempNum;
			cin >> tempNum;
			this->timeNameArr.push_back(tempName);
			this->timeArrSizeArr.push_back(tempNum);
			vector<person> tempArr;
			this->timeArrPtrArr.push_back(tempArr);
		}
	}
	else if (this->model == 1)
	{
		//数组的构建和获取各面试场次的人数
		for (int i = 0; i < tempTimeSize; i++)
		{
			cout << "请输入第" << i + 1 << "个时间段的名称<与表中首选面试时间的文字保持一致>（如：周六上午）：" << endl;
			string tempName;
			cin >> tempName;
			cout << "请输入" << tempName << "可以安排的数量的最小数量：" << endl;
			int tempMin;
			cin >> tempMin;
			cout << "请输入" << tempName << "可以安排的数量的最大数量：" << endl;
			int tempMax;
			cin >> tempMax;
			this->timeNameArr.push_back(tempName);
			this->timeArrSizeArr.push_back(tempMin);
			this->timeArrMaxSizeArr.push_back(tempMax);
			vector<person> tempArr;
			this->timeArrPtrArr.push_back(tempArr);
		}

		//遍历已经安排好的容器，确定还有空位的容器来进行搬运
		int index = 0;
		int nullNum = 0;
		for (vector<int>::iterator it = this->timeArrSizeArr.begin(); it != this->timeArrSizeArr.end(); it++)
		{
			nullNum = this->timeArrSizeArr[index];
			Size size(index, nullNum);
			this->availableArr.push_back(size);
			index++;
		}

		//对所有有空位的容器根据空余位置进行排序
		this->quickSort(this->availableArr, 0, this->availableArr.size() - 1);

		//自适应的策略
		for (vector<person>::iterator pit = this->sourceArr.begin(); pit != this->sourceArr.end(); pit++)
		{
			if (!this->adaptiveRun((*pit)))
			{
				if (!this->transfer(this->availableArr[0].index))
				{
					if ((this->timeArrMaxSizeArr[this->availableArr[0].index] > this->timeArrSizeArr[this->availableArr[0].index]))
					{
						this->timeArrSizeArr[this->availableArr[0].index]++;
						this->availableArr[0].nullNum++;
						this->adaptiveRun((*pit));
					}
					else
					{
						cout << "无法按照要求进行安排，请扩充【" << this->timeNameArr[this->availableArr[0].index] << "】的最大人数！" << endl;
						system("pause");
						exit(5);
					}
				}
				this->adaptiveRun((*pit));
			}
		}

	}
	else
	{
		cout << "模式选择错误，程序终止！" << endl;
		system("pause");
		exit(1);
	}

	//判断文件名是否正确
	ifstream ifs(tempFileName, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败，请检查文件名是否输入正确！" << endl;
		exit(2);
	}

	//读取掉第一行（标题）
	string tempLine;
	std::getline(ifs, tempLine);

	//开始录入人员数据
	while (this->sourceArr.size() < this->personSize)
	{
		//拆分表格
		person tempPerson;
		vector<string> words;
		string line;
		string word;
		istringstream sin;
		std::getline(ifs, line);
		sin.clear();
		sin.str(line);
		words.clear();
		while (std::getline(sin, word, ','))
		{
			words.push_back(word);
		}

		//录入
		tempPerson.name = words[0];
		for (int i = 0; i < this->timeSize; i++)
		{
			if (this->timeNameArr[i] == words[1])
			{
				tempPerson.mainTime = i;
			}
		}
		if (this->is_team == true)
		{
			tempPerson.teamNumber = words[2 + this->timeSize];
		}
		int count = 0;
		for (int i = 0; i < this->timeSize; i++)
		{
			if (words[i + 2] == FREE)
			{
				tempPerson.timeArr.push_back(true);
				count++;
			}
			else
			{
				tempPerson.timeArr.push_back(false);
			}
		}
		tempPerson.timeSize = this->timeSize;
		tempPerson.freeSize = count;
		this->sourceArr.push_back(tempPerson);
	}
	ifs.close();
}

void AIFM::run()
{
	srand(unsigned(time(NULL)));
	//高优先级：按首选时间安排
	this->remainPersonSize = this->personSize;
	for (int i = 0; i < this->personSize; i++)
	{
		if (this->timeArrSizeArr[this->sourceArr[i].mainTime] != this->timeArrPtrArr[this->sourceArr[i].mainTime].size())
		{
			this->sourceArr[i].is_arranged = true;
			this->sourceArr[i].is_main = true;
			this->timeArrPtrArr[this->sourceArr[i].mainTime].push_back(this->sourceArr[i]);
			this->remainPersonSize--;
		}
	}

	//低优先级：按空闲时间段的个数从少到多进行安排
	int totalTimes = 0;
	//超过一万次则自动退出
	while (this->remainPersonSize != 0) {
		totalTimes++;
		if (totalTimes == 10000)
		{
			cout << "已到达程序设定的最大运行次数！排表失败！请尝试其他参数！" << endl;
			system("pause");
			exit(3);
		}
		//依照空闲时间段数量从小到大来进行遍历，每一轮均遍历完所有成员
		for (int level = 1; level <= this->timeSize; level++)
		{
			int temp = 0;
			//i表示当前所在轮次，即为空闲时间段数量
			for (int i = 0; temp < this->remainPersonSize; i++)
			{
				//确定vector中的第一位空闲者位置
				while (this->sourceArr[i].is_arranged == true)
				{
					i++;
				}
				temp++;
				//若满足对应的轮次，则进入if
				if (this->sourceArr[i].freeSize == level)
				{
					//若空闲时间为1，则直接开始进行交换
					if (level == 1)
					{
						swap(this->sourceArr[i], this->timeArrPtrArr[this->sourceArr[i].mainTime]);
					}
					//收集这个人的空闲时间，存入vector中
					vector<int> tempTime;
					//j作为索引，指代数组中对应位置的时间段
					int j = 0;
					int count = this->sourceArr[i].freeSize;
					while (count != 0)
					{
						if (this->sourceArr[i].timeArr[j] == true)
						{
							tempTime.push_back(j);
							count--;
						}
						j++;
					}
					for (int x = 0; x < tempTime.size(); x++)
					{
						if (this->timeArrSizeArr[tempTime[x]] != this->timeArrPtrArr[tempTime[x]].size())
						{
							this->sourceArr[i].is_arranged = true;
							this->timeArrPtrArr[tempTime[x]].push_back(this->sourceArr[i]);
							this->remainPersonSize--;
							break;
						}
					}
					if (this->sourceArr[i].is_arranged == true)
					{
						break;
					}
					//进行随机交换，当交换次数超过某个设定值时（当前设定为50）则自动终止程序
					int times = 0;
					while (true)
					{
						int random = rand() % tempTime.size();
						if (swap(this->sourceArr[i], this->timeArrPtrArr[random]))
						{
							if (this->sourceArr[i].mainTime == random)
							{
								this->sourceArr[i].is_main = true;
							}
							break;
						}
						times++;
						if (times == 50)
						{
							cout << "无法合理分配人员到对应的时间段，请重设某些时间段的可安排数量！" << endl;
							system("pause");
							exit(4);
						}
					}
				}
			}
		}
	}
}

bool AIFM::swap(person& a, vector<person>& arr)
{
	//只有首选时间有空的情况
	if (a.freeSize == 1)
	{
		int maxSize = 0;
		vector<person>::iterator maxIndex;
		for (vector<person>::iterator it = arr.begin(); it != arr.end(); it++)
		{
			if ((*it).is_main != true)
			{
				a.is_arranged = true;
				a.is_main = true;
				(*it).is_arranged = false;
				(*it).is_main = false;
				arr.erase(it);
				arr.push_back(a);
				return true;
			}
			if ((*it).freeSize > maxSize)
			{
				maxSize = (*it).freeSize;
				maxIndex = it;
			}
		}
		a.is_arranged = true;
		a.is_main = true;
		(*maxIndex).is_arranged = false;
		(*maxIndex).is_main = false;
		arr.erase(maxIndex);
		arr.push_back(a);
		return true;
	}
	//三种不同情况的分类
	for (vector<person>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		if (a.freeSize < (*it).freeSize && (*it).is_main != true)
		{
			a.is_arranged = true;
			(*it).is_arranged = false;
			(*it).is_main = false;
			arr.erase(it);
			arr.push_back(a);
			return true;
		}
	}
	for (vector<person>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		if (a.freeSize == (*it).freeSize && (*it).is_main != true)
		{
			a.is_arranged = true;
			(*it).is_arranged = false;
			(*it).is_main = false;
			arr.erase(it);
			arr.push_back(a);
			return true;
		}
	}
	int maxSize = 0;
	vector<person>::iterator maxIndex;
	for (vector<person>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		if ((*it).freeSize > maxSize)
		{
			maxSize = (*it).freeSize;
			maxIndex = it;
		}
	}
	if (maxSize != 1)
	{
		a.is_arranged = true;
		(*maxIndex).is_arranged = false;
		(*maxIndex).is_main = false;
		arr.erase(maxIndex);
		arr.push_back(a);
		return true;
	}
	//这里的maxIndex存在较小的几率会导致程序陷入死循环，以后可以尝试优化一下，可以为执行最后一种可能的person增加一个remember成员，如果下次遇上remember则自动跳过
	//（思考：需不需要双向remember）
	return false;
	//false当且仅当全部都是首选且空闲时间段数量为1时才会触发
}

//自适应运行
bool AIFM::adaptiveRun(person& a)
{
	for (vector<Size>::iterator sit = this->availableArr.end(); sit != this->availableArr.begin(); sit--)
	{
		for (int i = 0; i < a.freeSize; i++)
		{
			//若成功匹配则进行安排，并调整available数组
			if (a.timeArr[i] == (*sit).index)
			{
				this->timeArrPtrArr[(*sit).index].push_back(a);
				a.is_arranged = true;
				(*sit).nullNum--;
				if (a.mainTime == (*sit).index)
				{
					a.is_main = true;
				}
				this->quickSort(this->availableArr, 0, this->availableArr.size() - 1);
				return true;
			}
		}
	}
	return false;
}

//快排的划分函数
int AIFM::partition(vector<Size>& arr, int low, int high)
{
	Size pivot = arr[low];
	while (low < high)
	{
		while (low<high && arr[high].nullNum > pivot.nullNum)
		{
			high--;
		}
		arr[low] = arr[high];
		while (low < high && arr[low].nullNum <= pivot.nullNum)
		{
			low++;
		}
		arr[high] = arr[low];
	}
	arr[low] = pivot;
	return low;
}

//快排
void AIFM::quickSort(vector<Size>& arr, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	else
	{
		int pivot = partition(arr, low, high);
		quickSort(arr, low, pivot - 1);
		quickSort(arr, pivot + 1, high);
	}
}

bool AIFM::transfer(int index)
{
	//index为初始数组在ptr数组中的下标
	vector<person> arr = this->timeArrPtrArr[index];
	//搬运目标数组在ptr数组中的下标
	int targetIndex = 0;
	//搬运目标数组在available数组中的下标
	int sizeIndex = this->availableArr.size() - 1;
	while (sizeIndex >= 0)
	{
		targetIndex = this->availableArr[sizeIndex].index;
		//若初始数组和目标数组相同则跳到下一次循环
		if (targetIndex == index)
		{
			sizeIndex--;
			continue;
		}

		//遍历初始数组，选取合适的person并搬运
		for (vector<person>::iterator pit = arr.begin(); pit != arr.end(); pit++)
		{
			if ((*pit).freeSize >= 2)
			{
				for (int i = 0; i <= this->timeSize; i++)
				{
					if ((*pit).timeArr[i] == targetIndex)
					{
						//搬运过程
						arr.erase(pit);
						this->timeArrPtrArr[targetIndex].push_back((*pit));
						if ((*pit).mainTime == targetIndex)
						{
							(*pit).is_main = true;
						}
						else
						{
							(*pit).is_main = false;
						}
						this->availableArr[0].nullNum++;
						this->availableArr[sizeIndex].nullNum--;

						//循环退出条件
						if (this->availableArr[0].nullNum >= this->availableArr[1].nullNum)
						{
							//对所有有空位的容器根据空余位置进行排序
							this->quickSort(this->availableArr, 0, this->availableArr.size() - 1);
							return true;
						}
						//对所有有空位的容器根据空余位置进行排序
						this->quickSort(this->availableArr, 0, this->availableArr.size() - 1);
					}
				}
			}
		}
		sizeIndex--;
	}
	return false;
}

void AIFM::output()
{
	ofstream ofs;
	cout << "请输入导出结果的文件名（后缀名为.csv）：";
	string tempFileName;
	cin >> tempFileName;
	ofs.open(tempFileName, ios::out | ios::trunc);
	ofs << "结果导出表（请务必仔细检查）" << endl;
	if (this->is_team == true)
	{
		ofs << "学校名称" << ',' << "竞争校队数量" << endl;
	}
	else
	{
		ofs << "姓名" << endl;
	}
	for (int i = 0; i < this->timeSize; i++)
	{
		ofs << "--[" << this->timeNameArr[i] << "]--" << endl;
		if (this->is_team == true)
		{
			for (int j = 0; j < this->timeArrPtrArr[i].size(); j++)
			{
				ofs << this->timeArrPtrArr[i][j].name << ',' << this->timeArrPtrArr[i][j].teamNumber << endl;
			}
		}
		else
		{
			for (int j = 0; j < this->timeArrPtrArr[i].size(); j++)
			{
				ofs << this->timeArrPtrArr[i][j].name << endl;
			}
		}
	}
	ofs.close();
	cout << "导出完毕！谢谢使用吖！" << endl;
	system("pause");
	return;
}

//展示安排好的人员
void AIFM::show1()
{
	for (int i = 0; i < this->timeSize; i++)
	{
		cout << this->timeNameArr[i] << "安排的人员有：" << endl;
		for (int j = 0; j < this->timeArrPtrArr[i].size(); j++)
		{
			cout << this->timeArrPtrArr[i][j].name << endl;
		}
	}
}

//展示录入好的数据
void AIFM::show2()
{
	for (int i = 0; i < this->personSize; i++)
	{
		cout << "姓名：" << this->sourceArr[i].name << endl << "首选时间：" << this->timeNameArr[this->sourceArr[i].mainTime] << endl;
		for (int j = 0; j < this->timeNameArr.size(); j++)
		{
			if (sourceArr[i].timeArr[j] == 1)
			{
				cout << this->timeNameArr[j] << "有空" << endl;
			}
			else
			{
				cout << this->timeNameArr[j] << "没空" << endl;
			}
		}
		if (this->is_team == true)
		{
			cout << "队伍人数:" << sourceArr[i].teamNumber << endl;
		}
	}
}