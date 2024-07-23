#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Person.h"
using namespace std;

class SpeechManager {
public:

	//构造函数
	SpeechManager();

	//析构函数
	~SpeechManager();

	//展示菜单
	void showMenu();

	//退出程序
	void exitSystem();

	//初始化成员属性
	void initial();

	//创建选手
	void createPerson();

	//比赛开始
	void startSpeech();

	//抽签
	void speechDraw();

	//比赛
	void speechContest();

	//显示比赛分数
	void showScore();

	//保存比赛分数
	void saveRecord();

	//读取往届比赛记录
	void loadRecord();

	//显示往届比赛得分
	void showRecord();

	//清空记录
	void clearRecord();

	vector<int> v1;

	vector<int> v2;

	vector<int> victory;

	map<int, Person> m_Person;

	int index;

	bool fileEmpty;

	map<int, vector<string>>  m_Record;
};
