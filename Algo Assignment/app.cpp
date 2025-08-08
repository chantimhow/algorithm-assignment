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
#define CLEAR_SCREEN system("cls");

  







using namespace std;

bool ReadFile(string filename, List* list);
bool DeleteRecord(List* list, char* id);
bool Display(List* list, int source, int detail);
bool InsertBook(string filename, List* list, Date &currentdate);
bool SearchStudent(List* list, char* id, LibStudent& studentinfo);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *,Date);
bool inputValidation(int min, int max, int input);
int menu();
time_t convertDate(Date time);
int compareDate(Date date1, Date date2);












int main() {
	

	Date currentdate;
	currentdate.day = 29; currentdate.month = 3; currentdate.year = 2020;
	LibStudent student;
	List* libdata = new List();
	List* type1 = new List();
	List* type2 = new List();
	string studentfile = "student.txt",bookfile = "book.txt";
	string temp;
	char idfind[8];
	char* callnum;

	cout << "Welcome to UTAR Library portal!\n";
		
	do {
		cout << endl;
		system("pause");
		CLEAR_SCREEN;
		int choice = menu();
		CLEAR_SCREEN;
		switch (choice) { //switch case for choice
		case 1:
			if (ReadFile(studentfile, libdata)) cout << "read successful\n";
			else cout << "read unsuccessful\n";
			break;


		case 2:
			cout << "enter student id to delete!\n";
			cin >> temp;
			if (strlen(temp.c_str()) > 7) {
				do {
					cout << "student letter consist of 7 digits!\n"; // to make sure that the id inserted is more than 7 digits
					cin >> temp;
				} while (strlen(temp.c_str()) > 7);
			}
			strcpy(idfind, temp.c_str());
			if (DeleteRecord(libdata, idfind)) cout << "delete successful!\n";
			else cout << "no ID in database match with ID entered,delete unsuccessful!\n";
			break;


		case 3:
			cout << "enter student id to search!\n";
			cin >> temp;
			if (strlen(temp.c_str()) > 7) {
				do {
					cout << "student letter consist of 7 digits!\n"; 
					cin >> temp;
				} while (strlen(temp.c_str()) > 7);
			}
			strcpy(idfind, temp.c_str());
			if (SearchStudent(libdata, idfind, student)) {
				student.print(cout);
			}
			else cout << "student not found!\n";
			break;


		case 4:
			if (InsertBook(bookfile, libdata, currentdate)) cout << "Inserted successful!\n";
			else cout << "unable to insert!\n";
			break;


		case 5:
			int source;
			int detail;
			cout << "1.Display to file\n" << "2.Display to screen\n";
			do {
				cin >> source;
			} while (!inputValidation(1, 2, source));
			cout << "1.Display student info and book info\n" << "2.Display student info only\n";
			do {
				cin >> detail;
			} while (!inputValidation(1, 2, detail));
			CLEAR_SCREEN;
			if (Display(libdata, source, detail)) cout << "display successful\n!";
			else cout << "unable to display\n!";
			break;

		case 6:
			if (computeAndDisplayStatistics(libdata)) cout << "compute successful!\n";
			else cout << "unable to compute!\n";
			break;


		case 7:
			cout << "please enter callNum!\n";
			cin >> temp;
			callnum = new char[strlen(temp.c_str()) + 1];
			strcpy(callnum, temp.c_str());
			if (!printStuWithSameBook(libdata, callnum)) cout << "student not found!\n";
			break;
		case 8:
			if (!displayWarnedStudent(libdata, type1, type2, currentdate)) cout << "no warned students!\n";
			break;

		case 9:
			cout << "good bye!\n";
			return 0;
		}
	} while (1);
	return 0;
}


bool ReadFile(string filename, List* list) {
	ifstream infile;
	string tempostring;
	LibStudent studentinfo;
	bool exist;
	size_t pos;
	int linepos = 0;


	infile.open(filename);
	if (!infile.is_open()) return false;
		
	while (getline(infile, tempostring)) {
		if (tempostring.find("Student Id") != string::npos) { // to find the identifiers and taking the value after identifier.
			pos = tempostring.find('='); 
			if (strlen(tempostring.substr(pos + 2).c_str()) < 10) { // pos + 2 as the value always starts two position after =, < 10 to prevent overflowing when strpying into studentinfo.id
				strcpy(studentinfo.id, tempostring.substr(pos + 2).c_str());
				linepos++;
			}
		}
		else if (tempostring.find("Name") != string::npos) {
			pos = tempostring.find('=');
			if (strlen(tempostring.substr(pos + 2).c_str()) < 30) {
				strcpy(studentinfo.name, tempostring.substr(pos + 2).c_str());
				linepos++;
			}
		}
		else if (tempostring.find("course") != string::npos) {
			pos = tempostring.find('=');
			if (strlen(tempostring.substr(pos + 2).c_str()) < 3) {
				strcpy(studentinfo.course, tempostring.substr(pos + 2).c_str());
				linepos++;
			}
		}
		else if (tempostring.find("Phone Number") != string::npos) {
			pos = tempostring.find('=');
			if (strlen(tempostring.substr(pos + 2).c_str()) < 10) {
				strcpy(studentinfo.phone_no, tempostring.substr(pos + 2).c_str());
				linepos++;
			}
		}
		if (linepos!= 0&& linepos % 4 == 0) { // to make sure all neccesary values are stored before inserting into the list
			exist = false;
				for (int i=1; i <= list->count; i++) {
					if (strcmp(list->find(i)->item.id,studentinfo.id)==0) {
						cout << "duplicate record found for ID (" << studentinfo.id <<") IGNORING RECORD...\n"; // to check whether the id already exist in the list.
						exist = true;
						break;
					}
					
				}
				if (!exist) {
					list->insert(studentinfo);
				}
			
				linepos = 0;
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
	if (list->empty()) return false;
	LibBook tempobooks =  LibBook();
	ifstream infile;
	infile.open(filename);
	if (!infile.is_open()) return false;
	char tempostring[100];
	char* token;
	bool bookexist = false;

	
	while (infile >> tempostring) {//id
		if (tempostring[0] == '\0') continue; // if tempostring is a empty skips everything and continue the loop
		for (int i = 1; i <= list->count; i++) {
			if (strcmp(list->find(i)->item.id, tempostring) == 0) {
				infile >> tempostring;
				token=strtok(tempostring, "/"); // strtok to replace '/' in tempostring to null and token is pointing to the first letter of tempostring
				for (int c = 0; token != NULL; c++) { //if token is null there is no more author in tempostring
					tempobooks.author[c] = new char[strlen(token) + 1]; // allocating memory to tempobooks.author with the length of token plus 1 for \0
					strcpy(tempobooks.author[c],token);
					token = strtok(NULL, "/");
				}
				infile >> tempobooks.title;
				infile >> tempobooks.publisher;
				infile >> tempobooks.ISBN;
				infile >> tempobooks.yearPublished;
				infile >> tempobooks.callNum;
				infile >> tempostring;
				tempobooks.borrow.day = atoi(strtok(tempostring, "/")); // same technique as the author, atoi to convert the cstring into int to be stored into tempobooks.borrow.day
				tempobooks.borrow.month = atoi(strtok(NULL, "/"));
				tempobooks.borrow.year = atoi(strtok(NULL, "/"));
				infile >> tempostring;
				tempobooks.due.day = atoi(strtok(tempostring, "/"));
				tempobooks.due.month = atoi(strtok(NULL, "/"));
				tempobooks.due.year = atoi(strtok(NULL, "/"));
				tempobooks.fine = (compareDate(currentdate, tempobooks.due) * 0.5); // a function to compare two dates and return their differences * 0.5 as one day fine is rm0.50
				for (int j = 0; j < list->find(i)->item.totalbook; j++) {
					if (strcmp(list->find(i)->item.book[j].callNum, tempobooks.callNum) == 0) {
						cout << "Duplicate records found for book with callnum (" << tempobooks.callNum << ") IGNORING RECORD... \n"; // to check for duplicate records in a student booklist by checking call numbers.
						tempobooks = LibBook();
						bookexist = true;
						break;
					}
				}
				if (!bookexist) {
					for (int c = 0; tempobooks.author[c] != NULL; c++) {
						list->find(i)->item.book[list->find(i)->item.totalbook].author[c] = new char[strlen(tempobooks.author[c])];
						strcpy(list->find(i)->item.book[list->find(i)->item.totalbook].author[c], tempobooks.author[c]);
					}
					strcpy(list->find(i)->item.book[list->find(i)->item.totalbook].title,tempobooks.title);
					strcpy(list->find(i)->item.book[list->find(i)->item.totalbook].publisher,tempobooks.publisher);
					strcpy(list->find(i)->item.book[list->find(i)->item.totalbook].ISBN,tempobooks.ISBN);
					list->find(i)->item.book[list->find(i)->item.totalbook].yearPublished=tempobooks.yearPublished;
					strcpy(list->find(i)->item.book[list->find(i)->item.totalbook].callNum,tempobooks.callNum);
					list->find(i)->item.book[list->find(i)->item.totalbook].borrow.day=tempobooks.borrow.day;
					list->find(i)->item.book[list->find(i)->item.totalbook].borrow.month=tempobooks.borrow.month;
					list->find(i)->item.book[list->find(i)->item.totalbook].borrow.year=tempobooks.borrow.year;
					list->find(i)->item.book[list->find(i)->item.totalbook].due.day=tempobooks.due.day;
					list->find(i)->item.book[list->find(i)->item.totalbook].due.month=tempobooks.due.month;
					list->find(i)->item.book[list->find(i)->item.totalbook].due.year=tempobooks.due.year;
					list->find(i)->item.book[list->find(i)->item.totalbook].fine=tempobooks.fine;
					++list->find(i)->item.totalbook;
					list->find(i)->item.calculateTotalFine();
					

				}

				
			}
		}
	}
	return true;
}


time_t convertDate(Date time) { // a time t function from the c_time library that converts time to a value that c_time understands.
	struct tm tm_time;
	tm_time.tm_year = time.year - 1900;
	tm_time.tm_mon = time.month - 1;
	tm_time.tm_mday = time.day;
	tm_time.tm_hour = 0;
	tm_time.tm_min = 0;
	tm_time.tm_sec = 0;
	return mktime(&tm_time);

}


int compareDate(Date currentdate, Date duedate) { // a function that uses the convert date function and comparing both converted time and returning their difference
	time_t date1 = convertDate(currentdate);
	time_t date2 = convertDate(duedate);
	if (date1 <= date2) return 0;
	
	double diff = difftime(date1, date2);
	return diff /(60 * 60 * 24); // 
}


bool Display(List* list, int source, int detail) {
	if (list->empty()) return false;
	ofstream outfile;

	switch (source) { // a switch within a switch for source and detail
	case 1:
		switch (detail) {
		case 1:
			outfile.open("student_bookinfo.txt");
			if (!outfile.is_open()) return false;
			for (int i = 1; i <= list->count; i++) {
				outfile << endl << "STUDENT " << i << endl;
				list->find(i)->item.print(outfile);
				outfile << "BOOK LIST: \n";
				for (int j = 0; j < list->find(i)->item.totalbook; j++) {
					list->find(i)->item.book[j].print(outfile);
				}
			}
			outfile.close();
			break;

					
		case 2:
			outfile.open("student_info.txt"); 
			if (!outfile.is_open()) return false;
			
			for (int i = 1; i <= list->count; i++) {
				outfile << endl << "STUDENT " << i << endl;
				list->find(i)->item.print(outfile);
			}
			outfile.close();
			break;
		}
		break;

		
	case 2:
		switch (detail) {
		case 1:
			for (int i = 1; i <= list->count; i++) {
				cout << endl << "STUDENT " << i << endl;
				list->find(i)->item.print(cout);
				cout << "BOOK LIST:";
				for (int j = 0; j < list->find(i)->item.totalbook; j++) {
					list->find(i)->item.book[j].print(cout);
				}
				cout << endl <<setw(80) << setfill('*') << '*' << endl;
			}
			break;


		case 2:
			
			for (int i = 1; i <= list->count; i++) {
				cout << endl << "STUDENT " << i << endl;
				list->find(i)->item.print(cout);
			}
			break;
		}
		break;
	}
	return true;
}


bool computeAndDisplayStatistics(List* list) {
	if (list->empty()) return false;
	cout << setw(10) << setfill(' ') << left << "Course";
	cout << setw(25) << setfill(' ') << left << "Number of Students";
	cout << setw(25) << setfill(' ') << left << "Total Borrowed Books";
	cout << setw(25) << setfill(' ') << left << "Total Overdue Books";
	cout << setw(25) << setfill(' ') << left << "Total Overdue Fine (RM)" << endl;
	

	auto print = [&list](const char *courseID) { //lamba function to print to reduce redundancy 
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
	if (list->empty()) return false;
	int count = 0;
	for (int i = 1; i <= list->count; i++) {
		for (int j = 0; j < list->find(i)->item.totalbook; j++) {
			if (strcmp(list->find(i)->item.book[j].callNum,booknumber)==0) {
				count++;
				break;
			}
		}
	}

	cout << "There are " << count << " students that borrow the book with call number " << booknumber << " as shown below:" << endl << endl;
	for (int i = 1; i <= list->count; i++) {

		for (int j = 0; j < list->find(i)->item.totalbook; j++) {
			if (strcmp(list->find(i)->item.book[j].callNum, booknumber) == 0) {
				cout << setw(100) << setfill('-') << '-' << endl;
				cout << "Student ID: " << list->find(i)->item.id<<endl;
				cout << "Name: " << list->find(i)->item.name<<endl;
				cout << "Course: " << list->find(i)->item.course<<endl;
				cout << "Phone Number: " << list->find(i)->item.phone_no<<endl;
				cout << "Borrow Date: "; 
				list->find(i)->item.book[j].borrow.print(cout);
				cout << endl << "Due Date: "; 
				list->find(i)->item.book[j].due.print(cout);
				cout << endl;
				

				
				break;
			}
		}
	}
	return true;
}

bool displayWarnedStudent(List*list, List* type1, List* type2,Date currentdate) {
	if (list->empty()) return false;
	for (int i = 1; i < list->count; i++) {
		int duebookcount = 0;
		bool exist1 = false;
		bool exist2 = false;
		bool allbooksdue = true;
		for (int j = 0; j < list->find(i)->item.totalbook; j++) { //to check how many books a student have that is due
			if (list->find(i)->item.book[j].fine >= 5) {
				duebookcount++;
			}
			if (list->find(i)->item.book[j].fine == 0) { // to check whether students books are all due
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
			if (type1->empty() && type2->empty()) return false; 
			if (!type1->empty()) {
				cout << "students with 2 books that are overdue and more than 10 days\n";
				for (int i = 1; i <= type1->count; i++) {
					type1->find(i)->item.print(cout);
					for (int j = 0; j < type1->find(i)->item.totalbook; j++) {
						type1->find(i)->item.book[j].print(cout);
					}
				}

			}
			if (!type2->empty()) {
				cout << "student with fine of more than RM50.00 and all books are over due\n";
				for (int i = 1; i <= type2->count; i++) {
					type2->find(i)->item.print(cout);
					for (int j = 0; j < type2->find(i)->item.totalbook; j++) {
						type2->find(i)->item.book[j].print(cout);
					}
				}
			}
	return true;
}


int menu(){
	int userinput = 0;
	cout << "1.Read File\n";
	cout << "2.Delete Record\n";
	cout << "3.Search Student\n";
	cout << "4.Insert Book\n";
	cout << "5.Display Output\n";
	cout << "6.Compute and Display Statistics\n";
	cout << "7.Student with Same Book\n";
	cout << "8.Display Warned Student\n";
	cout << "9.Exit\n";
	
	do {
		cout << "CHOICE: ";
		cin >> noskipws >> userinput;
	} while (!inputValidation(1,9,userinput));
	return userinput;
}

bool inputValidation(int min, int max, int input) { // a input validating accepting three int value with the first being the min value user can enter and the second being the maximum value user can enter and the last is the input of user.
	if (cin.fail()) {// if user enter anything other than digits, cin would fail as it is a interger data type.
		cout << "enter only digits!\n\n";
		cin.clear();
		cin.ignore(1000, '\n'); // to clear input buffer
		return false;
	}
	if (cin.peek() != '\n') { // if user enter 1a it would store 1 and ignore A this is to ensure that the next line should be \n, if not then return false.
		cout << "enter only digits!\n\n";
		cin.clear();
		cin.ignore(1000, '\n');
		return false;
	}

	if (input < min || input > max) { // to check whether inputted value is outside min and max.
		cout << "only enter digits between " << min << " and " << max << endl << endl;
		cin.ignore();// clear input buffer.
		return false;
	}
	cin.ignore();
	return true;
}



				
			




		
	
	


		
		
			

	
