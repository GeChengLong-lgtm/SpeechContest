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
	cout << "************��ӭ�μ��ݽ�����**************"<< endl;
	cout << " ************1.��ʼ�ݽ�����**************" << endl;
	cout << " ************2.�鿴�����¼**************" << endl;
	cout << " ************3.��ձ�����¼**************" << endl;
	cout << " ************0.�˳���������**************" << endl;
	cout << "******************************************" << endl;
	cout << endl;
}

void SpeechManager::exitSystem() {

	cout << "��ӭ�´�ʹ��" << endl;
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

		string name = "ѡ��";
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

	//��ǩ����������ʾ�������
	speechDraw();

	//����
	speechContest();

	//��ʾ�������
	showScore();

	this->index++;

	//��ǩ����������ʾ�������
	speechDraw();

	//����
	speechContest();

	//��ʾ�������
	showScore();

	//����������
	saveRecord();

	this->initial();
	this->createPerson();
	this->loadRecord();

	cout << "���������ϣ�" << endl;
	system("pause");
	system("cls");
}

void SpeechManager::speechDraw() {

	cout << "�� << " << this->index << " >> �ֱ���ѡ�����ڳ�ǩ" << endl;
	cout << "---------------------" << endl;
	cout << "��ǩ������£�" << endl;
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

	cout << "--------�� " << this->index << " �ֱ�����ʽ��ʼ��------------" << endl;

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
			double score = (600 + rand() % 400) / 10.f;//60-100��double��
			d.push_back(score);
		}

		sort(d.begin(), d.end(), greater<double>());
		d.pop_back();
		d.pop_front();

		double sum = accumulate(d.begin(), d.end(), 0.0f);
		double avg = sum / (double)d.size();

		cout << "ѡ�ֱ��" << *it
			<< "ѡ������" << this->m_Person[*it].m_Name
			<< "ѡ�ֵ÷�" << avg << endl;

		this->m_Person[*it].m_Score[this->index - 1] = avg;
		groupScore.insert(make_pair(avg, *it));

		if (num % 6 == 0) {
			cout << "�� " << num / 6 << " ��������Σ�" << endl;
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin();
				it != groupScore.end(); it++) {
				cout << "ѡ�ֱ�ţ�" << it->second
					<< "ѡ��������" << this->m_Person[it->second].m_Name
					<< "ѡ�ֵ÷֣�" << this->m_Person[it->second].m_Score[this->index-1] << endl;
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

	cout << "----------�� " << this->index << " �ֱ�������-------------" << endl;
	system("pause");
}

void SpeechManager::showScore() {

	cout << "--------�� " << this->index << " �ֽ���ѡ�����£�------------"
		<< endl;
	vector<int> v;
	if (this->index == 1) {
		v = v2;
	}
	else {
		v = victory;
	}

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "ѡ�ֱ�ţ�" << *it
			<< "ѡ��������" << this->m_Person[*it].m_Name
			<< "ѡ�ֵ÷֣�" << this->m_Person[*it].m_Score[this->index - 1] << endl;
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
	cout << "�ļ��Ѿ�����¼!" << endl;

	this->fileEmpty = false;
}

void SpeechManager::loadRecord() {

	ifstream ifs("speech.csv", ios::in);

	if (!ifs.is_open()) {
		this->fileEmpty = true;
		cout << "�ļ�������" << endl;
		ifs.close();
		return;
	}

	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		cout << "�ļ�Ϊ��" << endl;
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
		cout << "�ļ������ڻ��߼�¼Ϊ��" << endl;
	}
	/*for (int i = 0; i < this->m_Record.size(); i++) {
		for(vector<string>::iterator j = this->m_Record[i].begin(); j != this->m_Record[i].end(); j+=3)
		cout << "��" << i + 1 << "�����"
			<< "�ھ���ţ�" << this->m_Record[i]. 
			<< " " << "�Ǿ����" << this->m_Record[i][j+1] 
			<< " " << "������" << this->m_Record[i][j+2]
			<< endl;
	}*/
	else {
		for (int i = 0; i < this->m_Record.size(); i++) {
			cout << "��" << i + 1 << "�����"
				<< "�ھ���ţ�" << this->m_Record[i][0] << "�÷�:" << this->m_Record[i][1]
				<< " " << "�Ǿ����" << this->m_Record[i][2] << "�÷֣�" << this->m_Record[i][3]
				<< " " << "������" << this->m_Record[i][4] << "�÷֣�" << this->m_Record[i][5]
				<< endl;
		}
	}
		
	system("pause");
	system("cls");
}

void SpeechManager::clearRecord() {

	cout << "ȷ�������" << endl;
	cout << "1��ȷ��" << endl;
	cout << "2������" << endl;

	int select = 0;
	cin >> select;

	if (select == 1) {
		ofstream ofs("speech.csv", ios::trunc);
		ofs.close();
	}

	//��ʼ������
	this->initial();
	//����ѡ��
	this->createPerson();
	//��ȡ�����¼
	this->loadRecord();

	cout << "��ճɹ���" << endl;
}

int main() {

	srand((unsigned int)time(NULL));

	SpeechManager spm;

	int choice = 0;

	while (1) {
		spm.showMenu();

		cout << "���������ѡ��:" << endl;
		cin >> choice;

		/*
		for (map<int, Person>::iterator it = spm.m_Person.begin();
			it != spm.m_Person.end(); it++) {
			cout << "ѡ�ֱ�ţ�" << it->first
				<< " ѡ��������" << it->second.m_Name
				<< " ѡ�ַ�����" << it->second.m_Score[0] << endl;
		}
		*/
		switch (choice) {
		case 1: //��ʼ����
			spm.startSpeech();
			//system("cls");
			break;
		case 2: //�鿴��¼
			spm.showRecord();
			break;
		case 3: //��ռ�¼
			spm.clearRecord();
			break;
		case 0: //�˳�����
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