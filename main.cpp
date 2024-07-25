#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include<bits/stdc++.h>
using namespace std;

const double ACCOUNT_OPENING_FEE = 20.0;
const double SECURITY_DEPOSIT = 30.0;
const double BOOK_ISSUE_FEE = 2.0;
const double MIN_INITIAL_DEPOSIT = 50.0;

class Book {
public:
    string title;
    string author;
    int isbn;
    bool available;

    Book(string t, string a, int i) : title(t), author(a), isbn(i), available(true) {}
};

class Student {
public:
    int roll;
    string name;
    double balance;

    Student(int r, string n, double b) : roll(r), name(n), balance(b - ACCOUNT_OPENING_FEE - SECURITY_DEPOSIT) {}

    void display() const {
        cout << "Roll No: " << roll << endl;
        cout << "Name: " << name << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "New balance: $" << fixed << setprecision(2) << balance << endl;
    }

    bool canAfford(double amount) const {
        return balance >= amount;
    }

    void deduct(double amount) {
        balance -= amount;
    }
};

vector<Student> students;
vector<Book> books;

void create_account();
void display(int roll);
void deposit_amount(int roll, double amount);
void issue_item(int roll);
void display_sorted();
int find_student(int roll);
int find_book(int isbn);
void add_book();
void edit_book();
void view_books();

int main() {
    // Add initial 15 books to the library (using placeholder data)
    for (int i = 0; i < 15; i++) {
        books.emplace_back("Title", "Author", 1000 + i);
    }

    int option;
    bool is_admin;
    string password;

    while (true) {
        cout << "Login as:\n1. Admin\n2. Student\n0. Exit\n";
        cin >> option;

        if (option == 0) {
            break;
        }

        is_admin = (option == 1);

        cout << "Enter password: ";
        cin >> password;

        if (password == "password") { // Simple password for demonstration
            if (is_admin) {
                cout << "Admin options:\n1. Add book\n2. Edit book\n3. View book status\n4. View enrolled students\n5. View student balance\n";
                cin >> option;

                switch (option) {
                    case 1: {
                        add_book();
                        break;
                    }
                    case 2: {
                        edit_book();
                        break;
                    }
                    case 3: {
                        view_books();
                        break;
                    }
                    case 4: {
                        display_sorted();
                        break;
                    }
                    case 5: {
                        int roll;
                        cout << "Enter student roll number: ";
                        cin >> roll;
                        display(roll);
                        break;
                    }
                }
            } else {
                int roll;
                cout << "Enter your roll number: ";
                cin >> roll;

                int index = find_student(roll);
                if (index == -1) {
                    cout << "Student not found. Create an account? (1. Yes / 2. No): ";
                    cin >> option;
                    if (option == 1) {
                        create_account();
                    }
                } else {
                    cout << "Student options:\n1. View balance\n2. Deposit amount\n3. Issue item\n";
                    cin >> option;

                    switch (option) {
                        case 1: {
                            students[index].display();
                            break;
                        }
                        case 2: {
                            double amount;
                            cout << "Enter the amount to deposit: ";
                            cin >> amount;
                            deposit_amount(roll, amount);
                            break;
                        }
                        case 3: {
                            issue_item(roll);
                            break;
                        }
                    }
                }
            }
        } else {
            cout << "Incorrect password.\n";
        }
    }

    return 0;
}

void create_account() {
    int roll;
    cout << "Enter roll number (BBRRRR format): ";
    cin >> roll;

    if (find_student(roll) != -1) {
        cout << "Account already exists for this roll number.\n";
        return;
    }

    string name;
    cout << "Enter student name: ";
    cin.ignore();
    getline(cin, name);

    double initial_deposit;
    cout << "Enter initial deposit amount ($50 minimum): ";
    cin >> initial_deposit;

    if (initial_deposit < MIN_INITIAL_DEPOSIT) {
        cout << "Initial deposit must be at least $50.\n";
        return;
    }

    students.emplace_back(roll, name, initial_deposit);
}

void display(int roll) {
    int index = find_student(roll);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    students[index].display();
}

void deposit_amount(int roll, double amount) {
    int index = find_student(roll);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    students[index].deposit(amount);
}

void issue_item(int roll) {
    int index = find_student(roll);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Available books:\n";
    for (size_t i = 0; i < books.size(); i++) {
        if (books[i].available) {
            cout << i + 1 << ". " << books[i].title << " by " << books[i].author << " (ISBN: " << books[i].isbn << ")\n";
        }
    }

    int choice;
    cout << "Enter the number of the book you want to issue (0 to cancel): ";
    cin >> choice;

    if (choice == 0) {
        return;
    }

    if (books[choice - 1].available && students[index].canAfford(BOOK_ISSUE_FEE)) {
        books[choice - 1].available = false;
        students[index].deduct(BOOK_ISSUE_FEE);
        cout << "Book issued successfully. New balance: $" << fixed << setprecision(2) << students[index].balance << endl;
    } else {
        cout << "Cannot issue the book. Insufficient balance or book is unavailable.\n";
    }
}

void display_sorted() {
    vector<Student> sorted_students = students;

    sort(sorted_students.begin(), sorted_students.end(), [](const Student &a, const Student &b) {
        return a.roll < b.roll;
    });

    for (const auto &student : sorted_students) {
        cout << student.roll << " - " << student.name << " - Balance: $" << fixed << setprecision(2) << student.balance << endl;
    }
}

int find_student(int roll) {
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].roll == roll) {
            return i;
        }
    }
    return -1;
}

int find_book(int isbn) {
    for (size_t i = 0; i < books.size(); i++) {
        if (books[i].isbn == isbn) {
            return i;
        }
    }
    return -1;
}

void add_book() {
    string title, author;
    int isbn;

    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);

    cout << "Enter book author: ";
    getline(cin, author);

    cout << "Enter book ISBN: ";
    cin >> isbn;

    if (find_book(isbn) != -1) {
        cout << "A book with this ISBN already exists.\n";
        return;
    }

    books.emplace_back(title, author, isbn);
}

void edit_book() {
    int isbn;
    cout << "Enter book ISBN to edit: ";
    cin >> isbn;
    int index = find_book(isbn);
    if (index == -1) {
        cout << "Book not found.\n";
        return;
    }

    cout << "Current book title: " << books[index].title << endl;
    cout << "Enter new book title: ";
    cin.ignore();
    getline(cin, books[index].title);

    cout << "Current book author: " << books[index].author << endl;
    cout << "Enter new book author: ";
    getline(cin, books[index].author);

    cout << "Book details updated.\n";
}

void view_books() {
    for (const auto &book : books) {
        cout << "Title: " << book.title << endl;
        cout << "Author: " << book.author << endl;
        cout << "ISBN: " << book.isbn << endl;
        cout << "Available: " << (book.available ? "Yes" : "No") << endl << endl;
    }
}
