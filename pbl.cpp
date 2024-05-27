#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>

using namespace std;

class Question {
private:
    string question;
    vector<string> options;
    char correctAnswer;

public:
    Question() {}
    Question(string q, vector<string> opts, char ans) : question(q), options(opts), correctAnswer(ans) {}

    void input() {
        cout << "Nhap cau hoi: ";
        cin.ignore();
        getline(cin, question);
        options.clear();
        for (int i = 0; i < 4; ++i) {
            string option;
            cout << "Nhap lua chon " << char('A' + i) << ": ";
            getline(cin, option);
            options.push_back(option);
        }
        while (true) {
            cout << "Nhap dap an dung (A/B/C/D): ";
            cin >> correctAnswer;
            correctAnswer = toupper(correctAnswer);
            if (correctAnswer >= 'A' && correctAnswer <= 'D') {
                break;
            } else {
                cout << "Dap an khong hop le! Vui long nhap lai.\n";
            }
        }
    }

    void display() const {
        cout << question << endl;
        for (int i = 0; i < options.size(); ++i) {
            cout << char('A' + i) << ". " << options[i] << endl;
        }
    }

    char getCorrectAnswer() const {
        return correctAnswer;
    }

    string getQuestion() const {
        return question;
    }

    vector<string> getOptions() const {
        return options;
    }
};

class Test {
private:
    vector<Question> questions;

public:
    void loadQuestions(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Không thể mở file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            string q = line;
            vector<string> opts;
            for (int i = 0; i < 4; ++i) {
                getline(file, line);
                opts.push_back(line);
            }
            getline(file, line);
            char ans = line[0];
            questions.push_back(Question(q, opts, ans));
        }

        file.close();
    }

    void saveQuestions(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Khong the mo file " << filename << endl;
            return;
        }

        for (const auto& q : questions) {
            file << q.getQuestion() << endl;
            for (const auto& opt : q.getOptions()) {
                file << opt << endl;
            }
            file << q.getCorrectAnswer() << endl;
        }

        file.close();
    }

    void addQuestion() {
        Question q;
        q.input();
        questions.push_back(q);
        saveQuestions("questions.txt");
    }

    void deleteQuestion() {
        int index;
        cout << "Nhap so thu tu cau muon xoa: ";
        cin >> index;
        if (index > 0 && index <= questions.size()) {
            questions.erase(questions.begin() + index - 1);
            saveQuestions("questions.txt");
        } else {
            cout << "So thu tu khong hop le!" << endl;
        }
    }

    void replaceQuestion() {
        int index;
        cout << "Nhap so thu tu cau muon thay the: ";
        cin >> index;
        if (index > 0 && index <= questions.size()) {
            Question q;
            q.input();
            questions[index - 1] = q;
            saveQuestions("questions.txt");
        } else {
            cout << "So thu tu cau khong hop le!" << endl;
        }
    }

    void generateTest(int num, const string& filename) const {
        if (num <= 0 || num > questions.size()) {
            cout << "So luong cau hoi khong hop le!" << endl;
            return;
        }

        vector<Question> shuffledQuestions = questions;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(shuffledQuestions.begin(), shuffledQuestions.end(), std::default_random_engine(seed));

        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Khong the mo file " << filename << endl;
            return;
        }

        file << "De thi trac nghiem:" << endl;
        for (int i = 0; i < num; ++i) {
            file << "Câu " << i + 1 << ":" << endl;
            file << shuffledQuestions[i].getQuestion() << endl;
            for (const auto& opt : shuffledQuestions[i].getOptions()) {
                file << opt << endl;
            }
            file << endl;
        }

        file.close();
    }

    void takeTest(const string& studentName) {
        int score = 0;
        int questionNumber = 1; 
        for (const auto& q : questions) {
            cout << "Cau hoi " << questionNumber << ":" << endl;
            q.display();
            char answer;
            cout << "Cau trả loi cua ban: ";
            cin >> answer;
            cin.ignore();
            answer = toupper(answer);
            if (answer == q.getCorrectAnswer()) {
                score++;
            }
            questionNumber++; 
        }
        cout << "Diem cua ban: " << score << "/" << questions.size() << endl;
        saveTestResult("results.txt", studentName, score);
    }

    void readResults(const string& filename) const {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Khong the mo file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }

    void saveTestResult(const string& filename, const string& studentName, int score) const {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cout << "Khong the mo file " << filename << endl;
            return;
        }
        file << studentName << " " << score << endl;
        file.close();
    }
};

class Student {
private:
    string name;
    int age;
    int MSSV;
    string sex;
    string className;


public:
    void input() {
        cout << "Nhap ten cua ban: ";
        cin.ignore();
        getline(cin, name);
        cout << "Nhap tuoi cua ban: ";
        cin >> age;
        cout << "Nhap ma so sinh vien cua ban: ";
        cin >> MSSV;
        cout << "Nhap gioi tinh cua ban:";
        cin.ignore();
        getline(cin,sex);
        cout << "Nhap ten lop cua ban: ";
        cin.ignore();
        getline(cin, className);
    }

    void display() const {
        cout << "Ten: " << name << "\nTuoi: " << age << "\nMa so sinh vien: "<< MSSV << "\nGioi tinh: " << sex  << "\nLop: " << className << endl;
    }

    string getName() const {
        return name;
    }
};

int main() {
   Test test;
    test.loadQuestions("questions.txt");

    while (true) {
        cout << "CHUONG TRINH QUAN LI THI TRAC NGHIEM TIENG ANH:\n";
        cout << "CHUC NANG:\n";
        cout << "1. Them cau hoi.\n";
        cout << "2. Xoa cau hoi.\n";
        cout << "3. Thay the cau hoi.\n";
        cout << "4. Tao de thi.\n";
        cout << "5. Lam bai thi.\n";
        cout << "6. Đoc ket qua.\n";
        cout << "7. Thoat\n";
        cout << "Ban chon: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            test.addQuestion();
        } else if (choice == 2) {
            test.deleteQuestion();
        } else if (choice == 3) {
            test.replaceQuestion();
        } else if (choice == 4) {
            int numQuestions;
            cout << "Nhap so luong cau hoi cho de thi: ";
            cin >> numQuestions;
            string examFilename;
            cout << "Nhap ten file cho de thi: ";
            cin >> examFilename;
            test.generateTest(numQuestions, examFilename);
        } else if (choice == 5) {
            Student student;
            student.input();
            test.takeTest(student.getName());
        } else if (choice == 6) {
            test.readResults("results.txt");
        } else if (choice == 7) {
            break;
        } else {
            cout << "Lua chon cua ban khong hop le. Vui long thu lai!." << endl;
        }
    }

    return 0;
}