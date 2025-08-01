#define		_CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string.h>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"
#include	<string>
#include	<ctime>
#include	<iomanip>






using namespace std;

bool ReadFile(string filename, List* list);
bool DeleteRecord(List* list, char* id);
bool Display(List* list, int source, int detail);
bool InsertBook(string filename, List* list, Date currentdate);
bool SearchStudent(List* list, char* id, LibStudent& studentinfo);
//bool computeAndDisplayStatistics(List *);
//bool printStuWithSameBook(List *, char *);
//bool displayWarnedStudent(List *, List *, List *);
//int menu();
time_t convertDate(Date time);
int compareDate(Date date1, Date date2);






;




int main() {
	

	Date currentdate;
	currentdate.day = 29; currentdate.month = 3; currentdate.year = 2020;
	cout << "test";
	cout << "\n\n";
	system("pause");
	LibStudent currstudent;
	//return 0;
	List* libdata = new List();
	string filename = "book.txt";
		if (ReadFile("student.txt", libdata)) cout << "read successful!\n";
		else cout << "unable to read!";
	InsertBook("book.txt", libdata, currentdate);
	Display(libdata, 1, 1);
		

		


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
bool InsertBook(string filename, List* list, Date currentdate) {
	int delicount = 0;
	int index = 0;
	ifstream infile;
	infile.open(filename);
	if (!infile.is_open()) return false;
	char tempostring[60];
	while (infile >> tempostring) {//id
		if (tempostring[0] == '\0') continue;
		for (int i = 1; i <= list->count; i++) {
			if (list->find(i)->item.id == tempostring) index = i;
		}
		infile >> tempostring;//author
		for (char c : tempostring) {
			if (c == '/') delicount++;
		}
		list->find(index)->item.book[list->find(index)->item.totalbook].author[0] = strtok(tempostring, "/");
		for (int i = 1; i < delicount+1; i++) {
			list->find(index)->item.book[list->find(index)->item.totalbook].author[i] = strtok(NULL, "/");
		}
		infile >> list->find(index)->item.book[list->find(index)->item.totalbook].title;
		infile >> list->find(index)->item.book[list->find(index)->item.totalbook].publisher;
		infile >> list->find(index)->item.book[list->find(index)->item.totalbook].ISBN;
		infile >> list->find(index)->item.book[list->find(index)->item.totalbook].yearPublished;
		infile >> list->find(index)->item.book[list->find(index)->item.totalbook].callNum;
		infile >> tempostring;
		list->find(index)->item.book[list->find(index)->item.totalbook].borrow.day = atoi(strtok(tempostring,"/"));
		list->find(index)->item.book[list->find(index)->item.totalbook].borrow.month = atoi(strtok(NULL, "/"));
		list->find(index)->item.book[list->find(index)->item.totalbook].borrow.year = atoi(strtok(NULL, "/"));
		infile >> tempostring;
		list->find(index)->item.book[list->find(index)->item.totalbook].due.day = atoi(strtok(tempostring, "/"));
		list->find(index)->item.book[list->find(index)->item.totalbook].due.month = atoi(strtok(NULL, "/"));
		list->find(index)->item.book[list->find(index)->item.totalbook].due.year = atoi(strtok(NULL, "/"));
		list->find(index)->item.book[list->find(index)->item.totalbook].fine = (compareDate(currentdate, list->find(index)->item.book[list->find(index)->item.totalbook].due))*0.5;
		list->find(index)->item.totalbook++;
		list->find(index)->item.calculateTotalFine();
		

	}
	


	
	return true;
}
time_t convertDate(Date time) {
	struct tm tm_time;
	tm_time.tm_year = time.year - 1900;
	tm_time.tm_mon = time.month - 1;
	tm_time.tm_mday = time.day;
	tm_time.tm_hour = 0;
	tm_time.tm_min = 0;
	tm_time.tm_sec = 0;
	return mktime(&tm_time);

}
int compareDate(Date date1, Date date2) {
	time_t currentdate = convertDate(date1);
	time_t duedate = convertDate(date2);
	if (currentdate < duedate) return 0;
	
	double diff = difftime(currentdate, duedate);
	return diff /(60 * 60 * 24);

}

bool Display(List* list, int source, int detail) {
	ofstream outfile;

	switch (source) {
	case 1:
		
		switch (detail) {
		case 1:
			outfile.open("student1.txt");
			if (!outfile.is_open()) return false;
			for (int i = 1; i <= list->count; i++) {
				outfile << endl << "STUDENT " + i << endl;
				outfile << endl << "Name:  " << list->find(i)->item.name << endl;
				outfile << "Id:  " << list->find(i)->item.id << endl;
				outfile << "Course:  " << list->find(i)->item.course << endl;
				outfile << "Phone No:  " << list->find(i)->item.phone_no << endl;
				outfile << "Total Fine:  RM" << list->find(i)->item.calculateTotalFine() << fixed << setprecision(2) << endl << endl;
				outfile << "BOOK LIST:";
				for (int j = 0; j < list->find(i)->item.totalbook; j++) {
					outfile << endl << endl << "Book " << j + 1 << endl << endl;
					outfile << "Title: " << list->find(i)->item.book[j].title << endl;
					outfile << "Author: " << list->find(i)->item.book[j].author << endl;
					outfile << "Publisher: " << list->find(i)->item.book[j].publisher << endl;
					outfile << "Year Published: " << list->find(i)->item.book[j].yearPublished << endl;
					outfile << "ISBN: " << list->find(i)->item.book[j].ISBN << endl;
					outfile << "Call Number: " << list->find(i)->item.book[j].callNum << endl;
					outfile << "Borrow Date: " << list->find(i)->item.book[j].borrow.day << '/' << list->find(i)->item.book[j].borrow.month << '/' << list->find(i)->item.book[j].borrow.year << endl;
					outfile << "Due Date: " << list->find(i)->item.book[j].due.day << '/' << list->find(i)->item.book[j].due.month << '/' << list->find(i)->item.book[j].due.year << endl;
					outfile << "Fine: " << list->find(i)->item.book[j].fine << endl;
				}
				outfile << "*" << setw(100) << setfill('*');
				
			}
			break;
					
		case 2:
			outfile.open("student1.txt"); 
			if (!outfile.is_open()) return false;
			for (int i = 1; i <= list->count; i++) {
				outfile << endl << "STUDENT " + i << endl;
				outfile << endl << "Name:  " << list->find(i)->item.name << endl;
				outfile << "Id:  " << list->find(i)->item.id << endl;
				outfile << "Course:  " << list->find(i)->item.course << endl;
				outfile << "Phone No:  " << list->find(i)->item.phone_no << endl;
				outfile << "Total Fine:  RM" << list->find(i)->item.calculateTotalFine() << fixed << setprecision(2) << endl << endl;
				outfile << "*" << setw(100) << setfill('*') << endl;
				
			}
			
			outfile.close();
			break;
		}
		break;
		
	case 2:
		switch (detail) {
		case 1:
			for (int i = 1; i <= list->count; i++) {
				cout << endl << "STUDENT " + i << endl;
				cout << endl << "Name:  " << list->find(i)->item.name << endl;
				cout << "Id:  " << list->find(i)->item.id << endl;
				cout << "Course:  " << list->find(i)->item.course << endl;
				cout << "Phone No:  " << list->find(i)->item.phone_no << endl;
				cout << "Total Fine:  RM" << list->find(i)->item.calculateTotalFine() << fixed << setprecision(2) << endl << endl;
				cout << "BOOK LIST:";
				for (int j = 0; j < list->find(i)->item.totalbook; j++) {
					cout << endl << endl << "Book " << j + 1 << endl << endl;
					cout << "Title: " << list->find(i)->item.book[j].title << endl;
					cout << "Author: " << list->find(i)->item.book[j].author << endl;
					cout << "Publisher: " << list->find(i)->item.book[j].publisher << endl;
					cout << "Year Published: " << list->find(i)->item.book[j].yearPublished << endl;
					cout << "ISBN: " << list->find(i)->item.book[j].ISBN << endl;
					cout << "Call Number: " << list->find(i)->item.book[j].callNum << endl;
					cout << "Borrow Date: " << list->find(i)->item.book[j].borrow.day << '/' << list->find(i)->item.book[j].borrow.month << '/' << list->find(i)->item.book[j].borrow.year << endl;
					cout << "Due Date: " << list->find(i)->item.book[j].due.day << '/' << list->find(i)->item.book[j].due.month << '/' << list->find(i)->item.book[j].due.year << endl;
					cout << "Fine: " << list->find(i)->item.book[j].fine << endl;
				}
				cout << "*" << setw(100) << setfill('*');

			}
			break;
		case 2:
			
			for (int i = 1; i <= list->count; i++) {
				cout << endl << "STUDENT " + i << endl;
				cout << endl << "Name:  " << list->find(i)->item.name << endl;
				cout << "Id:  " << list->find(i)->item.id << endl;
				cout << "Course:  " << list->find(i)->item.course << endl;
				cout << "Phone No:  " << list->find(i)->item.phone_no << endl;
				cout << "Total Fine:  RM" << list->find(i)->item.calculateTotalFine() << fixed << setprecision(2) << endl << endl;
				cout << "*" << setw(100) << setfill('*') << endl;
			}
			break;
		}
		break;
	}
}

		
	
	


		
		
			

	
