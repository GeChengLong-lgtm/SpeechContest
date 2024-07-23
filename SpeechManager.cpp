#include "speechManager.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <deque>
#include<numeric>
#include <fstream>
using namespace std;

SpeechManager::SpeechManager() {

	this->initial();

	this->createPerson();

	this->loadRecord();
}

SpeechManager::~SpeechManager() {

}

void SpeechManager::showMenu() {

	cout << "******************************************" << endl;
	cout << "************欢迎参加演讲比赛**************"<< endl;
	cout << " ************1.开始演讲比赛**************" << endl;
	cout << " ************2.查看往届记录**************" << endl;
	cout << " ************3.清空比赛记录**************" << endl;
	cout << " ************0.退出比赛程序**************" << endl;
	cout << "******************************************" << endl;
	cout << endl;
}

void SpeechManager::exitSystem() {

	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}

void SpeechManager::initial() {

	this->v1.clear();
	this->v2.clear();
	this->victory.clear();
	this->m_Person.clear();

	this->index = 1;
	this->m_Record.clear();
}

void SpeechManager::createPerson() {

	for (int i = 0; i < personNum; i++) {
		Person pe;

		string name = "选手";
		string nameIndex = to_string(i+1);
		name += nameIndex;

		pe.m_Name = name;

		for (int j = 0; j < times; j++) {
			pe.m_Score[j] = 0;
		}

		this->v1.push_back(i + 10001);

		this->m_Person.insert(make_pair(i + 10001, pe));
	}
}

void SpeechManager::startSpeech() {

	//抽签、比赛、显示晋级结果
	speechDraw();

	//比赛
	speechContest();

	//显示晋级结果
	showScore();

	this->index++;

	//抽签、比赛、显示晋级结果
	speechDraw();

	//比赛
	speechContest();

	//显示晋级结果
	showScore();

	//保存比赛结果
	saveRecord();

	this->initial();
	this->createPerson();
	this->loadRecord();

	cout << "本届比赛完毕！" << endl;
	system("pause");
	system("cls");
}

void SpeechManager::speechDraw() {

	cout << "第 << " << this->index << " >> 轮比赛选手正在抽签" << endl;
	cout << "---------------------" << endl;
	cout << "抽签结果如下：" << endl;
	if (this->index == 1) {

		random_shuffle(v1.begin(), v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
	else {

		random_shuffle(v2.begin(), v2.end());
		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}

	cout << "---------------------" << endl;
	system("pause");
	cout << endl;
}

void SpeechManager::speechContest() {

	cout << "--------第 " << this->index << " 轮比赛正式开始：------------" << endl;

	multimap<double, int, greater<int>> groupScore;

	int num = 0;
	vector<int> curVec;
	if (this->index == 1) {
		curVec = v1;
	}
	else {
		curVec = v2;
	}

	for (vector<int>::iterator it = curVec.begin(); it != curVec.end(); it++) {
		num++;

		deque<double> d;
		d.clear();
		for (int i = 0; i < 10; i++) {
			double score = (600 + rand() % 400) / 10.f;//60-100的double数
			d.push_back(score);
		}

		sort(d.begin(), d.end(), greater<double>());
		d.pop_back();
		d.pop_front();

		double sum = accumulate(d.begin(), d.end(), 0.0f);
		double avg = sum / (double)d.size();

		cout << "选手编号" << *it
			<< "选手姓名" << this->m_Person[*it].m_Name
			<< "选手得分" << avg << endl;

		this->m_Person[*it].m_Score[this->index - 1] = avg;
		groupScore.insert(make_pair(avg, *it));

		if (num % 6 == 0) {
			cout << "第 " << num / 6 << " 组比赛名次：" << endl;
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin();
				it != groupScore.end(); it++) {
				cout << "选手编号：" << it->second
					<< "选手姓名：" << this->m_Person[it->second].m_Name
					<< "选手得分：" << this->m_Person[it->second].m_Score[this->index-1] << endl;
			}

			int count = 0;
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin();
				it != groupScore.end() && count < 3; it++, count++) {
				if (this->index == 1) {
					v2.push_back((*it).second);
				}
				else {
					victory.push_back((*it).second);
				}
			}

			groupScore.clear();

			cout << endl;
		}
	}

	cout << "----------第 " << this->index << " 轮比赛结束-------------" << endl;
	system("pause");
}

void SpeechManager::showScore() {

	cout << "--------第 " << this->index << " 轮晋级选手如下：------------"
		<< endl;
	vector<int> v;
	if (this->index == 1) {
		v = v2;
	}
	else {
		v = victory;
	}

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "选手编号：" << *it
			<< "选手姓名：" << this->m_Person[*it].m_Name
			<< "选手得分：" << this->m_Person[*it].m_Score[this->index - 1] << endl;
	}

	cout << endl;
	system("pause");
	system("cls");
	this->showMenu();
}

void SpeechManager::saveRecord() {

	ofstream ofs;
	ofs.open("speech.csv", ios::out | ios::app);

	for (vector<int>::iterator it = victory.begin(); it != victory.end();
		it++) {
		ofs << *it << ","
			<< m_Person[*it].m_Score[times - 1] << ",";
	}
	ofs << endl;

	ofs.close();
	cout << "文件已经被记录!" << endl;

	this->fileEmpty = false;
}

void SpeechManager::loadRecord() {

	ifstream ifs("speech.csv", ios::in);

	if (!ifs.is_open()) {
		this->fileEmpty = true;
		cout << "文件不存在" << endl;
		ifs.close();
		return;
	}

	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		cout << "文件为空" << endl;
		this->fileEmpty = true;
		ifs.close();
		return;
	}
	
	this->fileEmpty = false;

	ifs.putback(ch);

	string data;
	int index = 0;
	while (ifs >> data) {
		vector<string> v;

		int pos = -1;
		int start = 0;

		while (1) {
			pos = data.find(",", start);
			if (pos == -1)
				break;
			string tmp = data.substr(start, pos - start);
			v.push_back(tmp);
			start = pos + 1;
		}

		this->m_Record.insert(make_pair(index, v));
		index++;
	}

	ifs.close();
}

void SpeechManager::showRecord() {

	if (this->fileEmpty) {
		cout << "文件不存在或者记录为空" << endl;
	}
	/*for (int i = 0; i < this->m_Record.size(); i++) {
		for(vector<string>::iterator j = this->m_Record[i].begin(); j != this->m_Record[i].end(); j+=3)
		cout << "第" << i + 1 << "届比赛"
			<< "冠军编号：" << this->m_Record[i]. 
			<< " " << "亚军编号" << this->m_Record[i][j+1] 
			<< " " << "殿军编号" << this->m_Record[i][j+2]
			<< endl;
	}*/
	else {
		for (int i = 0; i < this->m_Record.size(); i++) {
			cout << "第" << i + 1 << "届比赛"
				<< "冠军编号：" << this->m_Record[i][0] << "得分:" << this->m_Record[i][1]
				<< " " << "亚军编号" << this->m_Record[i][2] << "得分：" << this->m_Record[i][3]
				<< " " << "殿军编号" << this->m_Record[i][4] << "得分：" << this->m_Record[i][5]
				<< endl;
		}
	}
		
	system("pause");
	system("cls");
}

void SpeechManager::clearRecord() {

	cout << "确定情况？" << endl;
	cout << "1、确认" << endl;
	cout << "2、返回" << endl;

	int select = 0;
	cin >> select;

	if (select == 1) {
		ofstream ofs("speech.csv", ios::trunc);
		ofs.close();
	}

	//初始化属性
	this->initial();
	//创建选手
	this->createPerson();
	//获取往届记录
	this->loadRecord();

	cout << "清空成功！" << endl;
}

int main() {

	srand((unsigned int)time(NULL));

	SpeechManager spm;

	int choice = 0;

	while (1) {
		spm.showMenu();

		cout << "请输入你的选择:" << endl;
		cin >> choice;

		/*
		for (map<int, Person>::iterator it = spm.m_Person.begin();
			it != spm.m_Person.end(); it++) {
			cout << "选手编号：" << it->first
				<< " 选手姓名：" << it->second.m_Name
				<< " 选手分数：" << it->second.m_Score[0] << endl;
		}
		*/
		switch (choice) {
		case 1: //开始比赛
			spm.startSpeech();
			//system("cls");
			break;
		case 2: //查看记录
			spm.showRecord();
			break;
		case 3: //清空记录
			spm.clearRecord();
			break;
		case 0: //退出程序
			spm.exitSystem();
			break;
		default:
			system("cls");
			break;
		}
	}
	

	system("pause");
	return 0;
}