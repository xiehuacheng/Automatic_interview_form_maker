#include"AIFM.h"
//ʹ��ǰ����ؼ��һ������define�е������Ƿ���ȷ
#define FREE "A.����"
#define NOT_FREE "B.������"
using namespace std;

void AIFM::input()
{
	//������Ϣ�Ļ�ȡ
	cout << "�뽫�������ļ����в������ļ�������׺��Ϊ.csv����";
	string tempFileName;
	cin >> tempFileName;

	cout << "ѡ���ܣ�0�������ߣ�1������У��" << endl;
	bool tempTeam;
	cin >> tempTeam;

	cout << "��������������";
	int tempPersonSize;
	cin >> tempPersonSize;

	cout << "��������Ҫ���ŵ�ʱ���������";
	int tempTimeSize;
	cin >> tempTimeSize;

	//�������ݵĳ�ʼ��
	this->personSize = tempPersonSize;
	this->timeSize = tempTimeSize;
	this->is_team = tempTeam;

	//�����û�������ѡ����ȷ������ģʽ
	cout << "��ѡ������ģʽ��0����ͨģʽ��1������Ӧģʽ�����ݸ����ķ�Χ����ѡ�����У�" << endl;
	int tempModel;
	cin >> tempModel;

	this->model = tempModel;

	if (this->model == 0)
	{
		//����Ĺ����ͻ�ȡ�����Գ��ε�����
		for (int i = 0; i < tempTimeSize; i++)
		{
			cout << "�������" << i + 1 << "��ʱ��ε�����<�������ѡ����ʱ������ֱ���һ��>���磺�������磩��" << endl;
			string tempName;
			cin >> tempName;
			cout << "������" << tempName << "���԰��ŵ�������" << endl;
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
		//����Ĺ����ͻ�ȡ�����Գ��ε�����
		for (int i = 0; i < tempTimeSize; i++)
		{
			cout << "�������" << i + 1 << "��ʱ��ε�����<�������ѡ����ʱ������ֱ���һ��>���磺�������磩��" << endl;
			string tempName;
			cin >> tempName;
			cout << "������" << tempName << "���԰��ŵ���������С������" << endl;
			int tempMin;
			cin >> tempMin;
			cout << "������" << tempName << "���԰��ŵ����������������" << endl;
			int tempMax;
			cin >> tempMax;
			this->timeNameArr.push_back(tempName);
			this->timeArrSizeArr.push_back(tempMin);
			this->timeArrMaxSizeArr.push_back(tempMax);
			vector<person> tempArr;
			this->timeArrPtrArr.push_back(tempArr);
		}

		//�����Ѿ����źõ�������ȷ�����п�λ�����������а���
		int index = 0;
		int nullNum = 0;
		for (vector<int>::iterator it = this->timeArrSizeArr.begin(); it != this->timeArrSizeArr.end(); it++)
		{
			nullNum = this->timeArrSizeArr[index];
			Size size(index, nullNum);
			this->availableArr.push_back(size);
			index++;
		}

		//�������п�λ���������ݿ���λ�ý�������
		this->quickSort(this->availableArr, 0, this->availableArr.size() - 1);

		//����Ӧ�Ĳ���
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
						cout << "�޷�����Ҫ����а��ţ������䡾" << this->timeNameArr[this->availableArr[0].index] << "�������������" << endl;
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
		cout << "ģʽѡ����󣬳�����ֹ��" << endl;
		system("pause");
		exit(1);
	}

	//�ж��ļ����Ƿ���ȷ
	ifstream ifs(tempFileName, ios::in);
	if (!ifs.is_open())
	{
		cout << "�ļ���ʧ�ܣ������ļ����Ƿ�������ȷ��" << endl;
		exit(2);
	}

	//��ȡ����һ�У����⣩
	string tempLine;
	std::getline(ifs, tempLine);

	//��ʼ¼����Ա����
	while (this->sourceArr.size() < this->personSize)
	{
		//��ֱ��
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

		//¼��
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
	//�����ȼ�������ѡʱ�䰲��
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

	//�����ȼ���������ʱ��εĸ������ٵ�����а���
	int totalTimes = 0;
	//����һ������Զ��˳�
	while (this->remainPersonSize != 0) {
		totalTimes++;
		if (totalTimes == 10000)
		{
			cout << "�ѵ�������趨��������д������ű�ʧ�ܣ��볢������������" << endl;
			system("pause");
			exit(3);
		}
		//���տ���ʱ���������С���������б�����ÿһ�־����������г�Ա
		for (int level = 1; level <= this->timeSize; level++)
		{
			int temp = 0;
			//i��ʾ��ǰ�����ִΣ���Ϊ����ʱ�������
			for (int i = 0; temp < this->remainPersonSize; i++)
			{
				//ȷ��vector�еĵ�һλ������λ��
				while (this->sourceArr[i].is_arranged == true)
				{
					i++;
				}
				temp++;
				//�������Ӧ���ִΣ������if
				if (this->sourceArr[i].freeSize == level)
				{
					//������ʱ��Ϊ1����ֱ�ӿ�ʼ���н���
					if (level == 1)
					{
						swap(this->sourceArr[i], this->timeArrPtrArr[this->sourceArr[i].mainTime]);
					}
					//�ռ�����˵Ŀ���ʱ�䣬����vector��
					vector<int> tempTime;
					//j��Ϊ������ָ�������ж�Ӧλ�õ�ʱ���
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
					//���������������������������ĳ���趨ֵʱ����ǰ�趨Ϊ50�����Զ���ֹ����
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
							cout << "�޷����������Ա����Ӧ��ʱ��Σ�������ĳЩʱ��εĿɰ���������" << endl;
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
	//ֻ����ѡʱ���пյ����
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
	//���ֲ�ͬ����ķ���
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
	//�����maxIndex���ڽ�С�ļ��ʻᵼ�³���������ѭ�����Ժ���Գ����Ż�һ�£�����Ϊִ�����һ�ֿ��ܵ�person����һ��remember��Ա������´�����remember���Զ�����
	//��˼�����費��Ҫ˫��remember��
	return false;
	//false���ҽ���ȫ��������ѡ�ҿ���ʱ�������Ϊ1ʱ�Żᴥ��
}

//����Ӧ����
bool AIFM::adaptiveRun(person& a)
{
	for (vector<Size>::iterator sit = this->availableArr.end(); sit != this->availableArr.begin(); sit--)
	{
		for (int i = 0; i < a.freeSize; i++)
		{
			//���ɹ�ƥ������а��ţ�������available����
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

//���ŵĻ��ֺ���
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

//����
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
	//indexΪ��ʼ������ptr�����е��±�
	vector<person> arr = this->timeArrPtrArr[index];
	//����Ŀ��������ptr�����е��±�
	int targetIndex = 0;
	//����Ŀ��������available�����е��±�
	int sizeIndex = this->availableArr.size() - 1;
	while (sizeIndex >= 0)
	{
		targetIndex = this->availableArr[sizeIndex].index;
		//����ʼ�����Ŀ��������ͬ��������һ��ѭ��
		if (targetIndex == index)
		{
			sizeIndex--;
			continue;
		}

		//������ʼ���飬ѡȡ���ʵ�person������
		for (vector<person>::iterator pit = arr.begin(); pit != arr.end(); pit++)
		{
			if ((*pit).freeSize >= 2)
			{
				for (int i = 0; i <= this->timeSize; i++)
				{
					if ((*pit).timeArr[i] == targetIndex)
					{
						//���˹���
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

						//ѭ���˳�����
						if (this->availableArr[0].nullNum >= this->availableArr[1].nullNum)
						{
							//�������п�λ���������ݿ���λ�ý�������
							this->quickSort(this->availableArr, 0, this->availableArr.size() - 1);
							return true;
						}
						//�������п�λ���������ݿ���λ�ý�������
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
	cout << "�����뵼��������ļ�������׺��Ϊ.csv����";
	string tempFileName;
	cin >> tempFileName;
	ofs.open(tempFileName, ios::out | ios::trunc);
	ofs << "����������������ϸ��飩" << endl;
	if (this->is_team == true)
	{
		ofs << "ѧУ����" << ',' << "����У������" << endl;
	}
	else
	{
		ofs << "����" << endl;
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
	cout << "������ϣ�ллʹ��߹��" << endl;
	system("pause");
	return;
}

//չʾ���źõ���Ա
void AIFM::show1()
{
	for (int i = 0; i < this->timeSize; i++)
	{
		cout << this->timeNameArr[i] << "���ŵ���Ա�У�" << endl;
		for (int j = 0; j < this->timeArrPtrArr[i].size(); j++)
		{
			cout << this->timeArrPtrArr[i][j].name << endl;
		}
	}
}

//չʾ¼��õ�����
void AIFM::show2()
{
	for (int i = 0; i < this->personSize; i++)
	{
		cout << "������" << this->sourceArr[i].name << endl << "��ѡʱ�䣺" << this->timeNameArr[this->sourceArr[i].mainTime] << endl;
		for (int j = 0; j < this->timeNameArr.size(); j++)
		{
			if (sourceArr[i].timeArr[j] == 1)
			{
				cout << this->timeNameArr[j] << "�п�" << endl;
			}
			else
			{
				cout << this->timeNameArr[j] << "û��" << endl;
			}
		}
		if (this->is_team == true)
		{
			cout << "��������:" << sourceArr[i].teamNumber << endl;
		}
	}
}