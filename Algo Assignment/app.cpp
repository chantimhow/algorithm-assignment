#define		_CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"





using namespace std;

bool ReadFile(string filename, List* list);
bool DeleteRecord(List* list, char* id);
//bool Display(List, int, int);
//bool InsertBook(string, List *);
bool SearchStudent(List* list, char* id, LibStudent& studentinfo);
//bool computeAndDisplayStatistics(List *);
//bool printStuWithSameBook(List *, char *);
//bool displayWarnedStudent(List *, List *, List *);
//int menu();


int main() {
	
	cout << "test";
	cout << "\n\n";
	system("pause");
	LibStudent currstudent;
	//return 0;
	List* libdata = new List();
	if (ReadFile("student.txt", libdata)) cout << "read successful!\n";
	else cout << "unable to read!";
	for (int i = 1; i <=  libdata->count; i++) cout << libdata->find(i)->item.name << endl;
	
	int* x = new int;
	char* userinput = new char[20];
	strcpy(userinput, "120023311");
	char* userinput1 = new char[20];
	strcpy(userinput1, "1201237");
	if (DeleteRecord(libdata, userinput)) cout << "deleted successfull!\n";
	else cout << "delete unsucessfull!\n";
	for (int i = 1; i <= libdata->count; i++) cout << libdata->find(i)->item.name << endl;
	if (SearchStudent(libdata, userinput1, currstudent)) cout << "student found\n";
	else cout << "student not found\n";
	cout << currstudent.name;


}


bool ReadFile(string filename, List* list) {
	ifstream infile;
	string tempostring;
	LibStudent studentinfo;
	bool exist;
	size_t pos;
	int linepos = 0;


	infile.open(filename);
	if (!infile.is_open()) {
		cout << "file is not open!";
		return false;
	};
	while (getline(infile, tempostring)) {
		if (tempostring.find("Student Id") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.id, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		else if (tempostring.find("Name") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.name, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		else if (tempostring.find("course") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.course, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		else if (tempostring.find("Phone Number") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.phone_no, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		if (linepos!= 0&& linepos % 4 == 0) {
			exist = false;
			if (list->empty()) {
				list->insert(studentinfo);
				linepos = 0;

			}
			else {
				for (int i=1; i <= list->count; i++) {
					if (list->find(i)->item.id == studentinfo.id) {
						exist = true;
						break;
					}
				}
				if (!exist) {
					list->insert(studentinfo);
					linepos = 0;
				}
			}

		}
	}
	return true;
}
bool DeleteRecord(List* list, char* id) {
	if (list->empty()) return false;
	for (int i = 1; i <= list->count; i++) {
		if (strcmp(list->find(i)->item.id,id)==0) {
			if (list->remove(i)) return true;

		}

	}
	return false;
	
}

bool SearchStudent(List* list, char* id, LibStudent &studentinfo) {
	if (list->empty()) return false;
	for (int i = 1; i <= list->count; i++) {
		if (strcmp(list->find(i)->item.id, id)==0) {
			studentinfo = list->find(i)->item;
			return true;
		}

	}
	return false;

}

		
		
			

	
