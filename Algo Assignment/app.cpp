#define		_CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string.h>
#include	<string>
#include	<ctime>
#include	<iomanip>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"

  







using namespace std;

bool ReadFile(string filename, List* list);
bool DeleteRecord(List* list, char* id);
bool Display(List* list, int source, int detail);
bool InsertBook(string filename, List* list, Date &currentdate);
bool SearchStudent(List* list, char* id, LibStudent& studentinfo);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *,Date);
int menu();
time_t convertDate(Date time);
int compareDate(Date date1, Date date2);











int main() {
	

	Date currentdate;
	currentdate.day = 29; currentdate.month = 3; currentdate.year = 2020;
	cout << "test";
	cout << "\n\n";
	system("pause");
	
	//return 0;
	List* libdata = new List();
	List* type1 = new List();
	List* type2 = new List();

	string filename = "book.txt";
		if (ReadFile("student.txt", libdata)) cout << "read successful!\n";
		else cout << "unable to read!";
	InsertBook("book.txt", libdata, currentdate);
	cout << endl << endl;
	for (int i = 1; i <= libdata->count; i++) {
		libdata->find(i)->item.print(cout);
	}
	computeAndDisplayStatistics(libdata);
	
	//for (int i = 1; i < libdata->count; i++){
	//	if (strcmp(libdata->find(i)->item.id, "1201237") == 0) {
	//		for (int j = 0; j < libdata->find(i)->item.totalbook; j++) {
	//			cout << libdata->find(i)->item.book[j].fine << endl;
	//		}
	//	}
	//}
	char callid[50] = "QA76.73.C15K35";
	printStuWithSameBook(libdata, callid);
	displayWarnedStudent(libdata, type1, type2, currentdate);
	cout << libdata->find(1)->item.book[0].author[0];
		

		


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
			strcpy(studentinfo.id, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		else if (tempostring.find("Name") != string::npos) {
			pos = tempostring.find('=');
			strcpy(studentinfo.name, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		else if (tempostring.find("course") != string::npos) {
			pos = tempostring.find('=');
			strcpy(studentinfo.course, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		else if (tempostring.find("Phone Number") != string::npos) {
			pos = tempostring.find('=');
			strcpy(studentinfo.phone_no, tempostring.substr(pos + 2).c_str());
			linepos++;
		}
		if (linepos!= 0&& linepos % 4 == 0) {
			exist = false;
				for (int i=1; i <= list->count; i++) {
					if (strcmp(list->find(i)->item.id,studentinfo.id)==0) {
						exist = true;
						break;
					}
				}
				if (!exist) {
					list->insert(studentinfo);
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
bool InsertBook(string filename, List* list, Date &currentdate) {
	
	
	ifstream infile;
	infile.open(filename);
	if (!infile.is_open()) return false;
	char tempostring[60];
	char* token;
	while (infile >> tempostring) {//id
		if (tempostring[0] == '\0') continue;
		for (int i = 1; i <= list->count; i++) {
			if (strcmp(list->find(i)->item.id, tempostring) == 0) {
				infile >> tempostring;
				token = strtok(tempostring, "/");
				for (int c = 0; token != NULL; c++) {
					list->find(i)->item.book[list->find(i)->item.totalbook].author[c] = token;
					token = strtok(NULL, "/");
					
				}

				
				infile >> list->find(i)->item.book[list->find(i)->item.totalbook].title;
				infile >> list->find(i)->item.book[list->find(i)->item.totalbook].publisher;
				infile >> list->find(i)->item.book[list->find(i)->item.totalbook].ISBN;
				infile >> list->find(i)->item.book[list->find(i)->item.totalbook].yearPublished;
				infile >> list->find(i)->item.book[list->find(i)->item.totalbook].callNum;
				infile >> tempostring;
				list->find(i)->item.book[list->find(i)->item.totalbook].borrow.day = atoi(strtok(tempostring, "/"));
				list->find(i)->item.book[list->find(i)->item.totalbook].borrow.month = atoi(strtok(NULL, "/"));
				list->find(i)->item.book[list->find(i)->item.totalbook].borrow.year = atoi(strtok(NULL, "/"));
				infile >> tempostring;
				list->find(i)->item.book[list->find(i)->item.totalbook].due.day = atoi(strtok(tempostring, "/"));
				list->find(i)->item.book[list->find(i)->item.totalbook].due.month = atoi(strtok(NULL, "/"));
				list->find(i)->item.book[list->find(i)->item.totalbook].due.year = atoi(strtok(NULL, "/"));
				list->find(i)->item.book[list->find(i)->item.totalbook].fine = (compareDate(currentdate, list->find(i)->item.book[list->find(i)->item.totalbook].due)) * 0.5;
				++list->find(i)->item.totalbook;
				list->find(i)->item.calculateTotalFine();
			}
		}
		

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
int compareDate(Date currentdate, Date duedate) {
	time_t date1 = convertDate(currentdate);
	time_t date2 = convertDate(duedate);
	if (date1 <= date2) return 0;
	
	double diff = difftime(date1, date2);
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
	return true;
}
bool computeAndDisplayStatistics(List* list) {
	if (list == NULL) return false;
	cout << setw(10) << setfill(' ') << left << "Course";
	cout << setw(25) << setfill(' ') << left << "Number of Students";
	cout << setw(25) << setfill(' ') << left << "Total Borrowed Books";
	cout << setw(25) << setfill(' ') << left << "Total Overdue Books";
	cout << setw(25) << setfill(' ') << left << "Total Overdue Fine (RM)" << endl;
	

	auto print = [&list](const char *courseID) {
		int studentcount = 0;
		int totalbookdue = 0;
		double totalfine = 0;
		int totalbooks = 0;
		for (int i = 1; i <= list->count; i++) {
			if (strcmp(list->find(i)->item.course, courseID) == 0)
			{
				++studentcount;
				totalbooks += list->find(i)->item.totalbook;
				totalfine += list->find(i)->item.total_fine;
				for (int j = 0; j < list->find(i)->item.totalbook; j++) {
					if (list->find(i)->item.book[j].fine > 0) ++totalbookdue;
				}
			}
		}
		cout << setw(10) << setfill(' ') << left << courseID;
		cout << setw(25) << setfill(' ') << left << studentcount;
		cout << setw(25) << setfill(' ') << left << totalbooks;
		cout << setw(25) << setfill(' ') << left << totalbookdue;
		cout << setw(25) << setfill(' ') << left  << fixed << showpoint << setprecision(2) << totalfine << endl;
		};
	print("CS");
	print("IA");
	print("IB");
	print("CN");
	print("CT");
	return true;
}	

bool printStuWithSameBook(List *list, char* booknumber) {
	if (list == NULL) return false;
	int count = 0;
	for (int i = 1; i <= list->count; i++) {
		for (int j = 0; j < list->find(i)->item.totalbook; j++) {
			if (strcmp(list->find(i)->item.book[j].callNum,booknumber)==0) {
				count++;
				break;
			}
		}
	}
	cout << "There are " << count << " students that borrow the book with call number " << booknumber << " as shown below:";
	for (int i = 1; i <= list->count; i++) {

		for (int j = 0; j < list->find(i)->item.totalbook; j++) {
			if (strcmp(list->find(i)->item.book[j].callNum, booknumber) == 0) {
				list->find(i)->item.print(cout);
				break;
			}
		}
	}
	return true;
}
bool displayWarnedStudent(List*list, List* type1, List* type2,Date currentdate) {
	if (list == NULL) return false;
	for (int i = 1; i < list->count; i++) {
		int duebookcount = 0;
		bool exist1 = false;
		bool exist2 = false;
		bool allbooksdue=true;
		for (int j = 0; j < list->find(i)->item.totalbook; j++) {
			if (list->find(i)->item.book[j].fine >= 5) {
				duebookcount++;
			}
			if (list->find(i)->item.book[j].fine == 0) {
				allbooksdue = false;
				
			}

		}
			if (duebookcount > 2) {//type 1 typa student
				for (int v = 1; v <= type1->count; v++) {
					if (type1->find(v)->item.compareName2(list->find(i)->item)) {
						exist1 = true;
						break;
					}
				}
				if (!exist1) {
					type1->insert(list->find(i)->item);
				}
			}
			if (allbooksdue && list->find(i)->item.total_fine > 50) {
				for (int v = 1; v <= type2->count; v++) {
					if (type2->find(v)->item.compareName2(list->find(i)->item)) {
						exist2 = true;
						break;
					}
				}
				if (!exist2) {
					type2->insert(list->find(i)->item);
				}
				
			}
			
		
	}
	cout << "students with 2 books that are overdue and more than 10 days\n";
	for (int i = 1; i <= type1->count; i++) {
		type1->find(i)->item.print(cout);
		for (int j = 0; j < type1->find(i)->item.totalbook; j++) {
			type1->find(i)->item.book[j].print(cout);
		}
	}
	cout << "student with fine of more than RM50.00 and all books are over due\n";
	for (int i = 1; i <= type2->count; i++) {
		type2->find(i)->item.print(cout);
		for (int j = 0; j < type2->find(i)->item.totalbook; j++) {
			type2->find(i)->item.book[j].print(cout);
		}
	}
	return true;
}

int menu(){
	int userinput;
	cout << "1.Read File\n";
	cout << "2.Delete Record\n";
	cout << "3.Search Student\n";
	cout << "4.Insert Book\n";
	cout << "5.Display Output\n";
	cout << "6.Compute and Display Statistics\n";
	cout << "7.Student with Same Book\n";
	cout << "8.Display Warned Student\n";
	cout << "9.Exit\n";
	do{ 
		cin >> userinput;
		if (userinput <= 0 || userinput > 9) cout << "please only enter a value between 1 and 9!";
	}while (userinput <= 0 || userinput > 9);
	return userinput; 

	
}


				
			




		
	
	


		
		
			

	
