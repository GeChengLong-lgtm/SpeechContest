#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Person.h"
using namespace std;

class SpeechManager {
public:

	//���캯��
	SpeechManager();

	//��������
	~SpeechManager();

	//չʾ�˵�
	void showMenu();

	//�˳�����
	void exitSystem();

	//��ʼ����Ա����
	void initial();

	//����ѡ��
	void createPerson();

	//������ʼ
	void startSpeech();

	//��ǩ
	void speechDraw();

	//����
	void speechContest();

	//��ʾ��������
	void showScore();

	//�����������
	void saveRecord();

	//��ȡ���������¼
	void loadRecord();

	//��ʾ��������÷�
	void showRecord();

	//��ռ�¼
	void clearRecord();

	vector<int> v1;

	vector<int> v2;

	vector<int> victory;

	map<int, Person> m_Person;

	int index;

	bool fileEmpty;

	map<int, vector<string>>  m_Record;
};
