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
//bool DeleteRecord(List *, char *);
//bool Display(List, int, int);
//bool InsertBook(string, List *);
//bool SearchStudent(List *, char *id, LibStudent &);
//bool computeAndDisplayStatistics(List *);
//bool printStuWithSameBook(List *, char *);
//bool displayWarnedStudent(List *, List *, List *);
//int menu();


int main() {
	
	cout << "test";
	cout << "\n\n";
	system("pause");
	//return 0;
	List* libdata = new List();
	ReadFile("student.txt", libdata);
	

}


bool ReadFile(string filename, List* list) {
	ifstream infile;
	string tempostring;
	LibStudent studentinfo;
	int pos, linepos = 0;
	

	infile.open(filename);
	if (!infile.is_open()) {
		cout << "file is not open!";
		return false;
	};
	while (getline(infile, tempostring)) {
		if (tempostring.find("Student Id") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.id, tempostring.substr(pos + 2).c_str());
			if(list->empty()){

		}
		else if (tempostring.find("Name") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.name, tempostring.substr(pos + 2).c_str());
		}
		else if (tempostring.find("course") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.course, tempostring.substr(pos + 2).c_str());
		}
		else if (tempostring.find("Phone Number") != string::npos) {
			pos = tempostring.find('=');
			strcpy_s(studentinfo.phone_no, tempostring.substr(pos + 2).c_str());
		}
		linepos++;
		
			
		}// add more identifier check here
		if (list->empty()) {// to add the value checking here
			list->insert(studentinfo);
		}
		

	}
	return true;
}

	
