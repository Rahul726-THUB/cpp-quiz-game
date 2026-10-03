#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <iomanip>

using namespace std;

// ==========================================
// 1. UTILITY FUNCTION: STRING SPLITTER
// ==========================================
vector<string> splitLine(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(line);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ==========================================
// 2. QUESTION MANAGEMENT MODULE
// ==========================================
class Question {
private:
    string category;
    int difficulty;
    string questionText;
    string options[4];
    int correctOption;

public:
    Question(string cat, int diff, string q, string o1, string o2, string o3, string o4, int correct) {
        category = cat;
        difficulty = diff;
        questionText = q;
        options[0] = o1; options[1] = o2; options[2] = o3; options[3] = o4;
        correctOption = correct;
    }

    // Displays question and measures time taken to answer
    int askQuestionWithTimer(int timeLimitSeconds) {
        cout << "\n[" << category << " | Difficulty: " << difficulty << "] " << questionText << endl;
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ". " << options[i] << endl;
        }

        auto start = chrono::steady_clock::now(); // Start Timer
        
        int choice;
        cout << "Enter your choice (1-4) within " << timeLimitSeconds << " seconds: ";
        cin >> choice;

        auto end = chrono::steady_clock::now(); // End Timer
        int elapsedSeconds = chrono::duration_cast<chrono::seconds>(end - start).count();

        cout << "Time taken: " << elapsedSeconds << " seconds." << endl;

        if (elapsedSeconds > timeLimitSeconds) {
            cout << "Time's up! No points awarded." << endl;
            return 0; // 0 points
        } else if (choice == correctOption) {
            cout << "Correct! (+10 points)" << endl;
            return 10; // 10 points
        } else {
            cout << "Wrong answer." << endl;
            return 0;
        }
    }
    
    string getCategory() { return category; }
};

// ==========================================
// 3. AUTHENTICATION & ROLE MODULE (Polymorphism)
// ==========================================
class User {
protected:
    string username;
public:
    User(string uname) : username(uname) {}
    virtual void displayMenu() = 0; // Pure virtual function
    virtual ~User() {}
};

class Player : public User {
public:
    Player(string uname) : User(uname) {}

    void displayMenu() override {
        cout << "\n--- Player Menu ---" << endl;
        cout << "1. Start Quiz" << endl;
        cout << "2. Exit" << endl;
        
        int choice;
        cout << "Select option: ";
        cin >> choice;

        if (choice == 1) {
            playQuiz();
        }
    }

    void playQuiz() {
        vector<Question> quizQuestions;
        ifstream file("questions.txt");
        string line;
        
        // Load questions from file
        while (getline(file, line)) {
            vector<string> data = splitLine(line, '|');
            if (data.size() == 8) {
                quizQuestions.push_back(Question(data[0], stoi(data[1]), data[2], data[3], data[4], data[5], data[6], stoi(data[7])));
            }
        }
        file.close();

        if (quizQuestions.empty()) {
            cout << "No questions available in the database!" << endl;
            return;
        }

        int score = 0;
        cout << "\n--- Quiz Starting! ---" << endl;
        for (Question& q : quizQuestions) {
            score += q.askQuestionWithTimer(15); // 15 second time limit
        }

        cout << "\nQuiz Finished! Total Score: " << score << endl;

        // Save Score
        ofstream scoreFile("scores.txt", ios::app);
        scoreFile << username << "|" << score << endl;
        scoreFile.close();
    }
};

class Admin : public User {
public:
    Admin(string uname) : User(uname) {}

    void displayMenu() override {
        int choice = 0;
        while (choice != 3) {
            cout << "\n--- Admin Menu ---" << endl;
            cout << "1. Add New Question" << endl;
            cout << "2. View Score Analytics" << endl;
            cout << "3. Logout" << endl;
            cout << "Select option: ";
            cin >> choice;

            if (choice == 1) addQuestion();
            if (choice == 2) viewAnalytics();
        }
    }

    void addQuestion() {
        ofstream file("questions.txt", ios::app);
        string cat, q, o1, o2, o3, o4;
        int diff, correct;
        
        cin.ignore(); // clear input buffer
        cout << "Enter Category (e.g., C++, AI): "; getline(cin, cat);
        cout << "Enter Difficulty (1-3): "; cin >> diff; cin.ignore();
        cout << "Enter Question: "; getline(cin, q);
        cout << "Option 1: "; getline(cin, o1);
        cout << "Option 2: "; getline(cin, o2);
        cout << "Option 3: "; getline(cin, o3);
        cout << "Option 4: "; getline(cin, o4);
        cout << "Correct Option (1-4): "; cin >> correct;

        file << cat << "|" << diff << "|" << q << "|" << o1 << "|" << o2 << "|" << o3 << "|" << o4 << "|" << correct << endl;
        file.close();
        cout << "Question added successfully!" << endl;
    }

    void viewAnalytics() {
        ifstream file("scores.txt");
        string line;
        vector<int> scores;
        
        while (getline(file, line)) {
            vector<string> data = splitLine(line, '|');
            if (data.size() == 2) {
                scores.push_back(stoi(data[1]));
            }
        }
        file.close();

        if (scores.empty()) {
            cout << "No score data available for analysis." << endl;
            return;
        }

        // Calculating summary statistics for dataset preprocessing
        int minScore = scores[0], maxScore = scores[0], sum = 0;
        for (int s : scores) {
            if (s < minScore) minScore = s;
            if (s > maxScore) maxScore = s;
            sum += s;
        }
        
        double mean = (double)sum / scores.size();
        
        // Standard Deviation calculation
        double varianceSum = 0;
        for (int s : scores) {
            varianceSum += pow(s - mean, 2);
        }
        double stdDev = sqrt(varianceSum / scores.size());

        cout << "\n--- Player Analytics ---" << endl;
        cout << "Total Attempts : " << scores.size() << endl;
        cout << "Maximum Score  : " << maxScore << endl;
        cout << "Minimum Score  : " << minScore << endl;
        cout << "Mean Score     : " << fixed << setprecision(2) << mean << endl;
        cout << "Std Deviation  : " << stdDev << endl;
    }
};

// ==========================================
// 4. MAIN EXECUTION ENGINE
// ==========================================
int main() {
    cout << "--- C++ Quiz Platform ---" << endl;
    string inputUser, inputPass;
    
    cout << "Username: ";
    cin >> inputUser;
    cout << "Password: ";
    cin >> inputPass;

    ifstream userFile("users.txt");
    string line;
    bool loggedIn = false;
    User* currentUser = nullptr;

    // Authentication matching
    while (getline(userFile, line)) {
        vector<string> data = splitLine(line, '|');
        if (data.size() == 3) {
            if (data[0] == inputUser && data[1] == inputPass) {
                loggedIn = true;
                if (data[2] == "admin") {
                    currentUser = new Admin(inputUser);
                } else {
                    currentUser = new Player(inputUser);
                }
                break;
            }
        }
    }
    userFile.close();

    if (loggedIn && currentUser != nullptr) {
        cout << "\nLogin successful!" << endl;
        currentUser->displayMenu(); // Polymorphic call based on role
        delete currentUser; // Clean up memory
    } else {
        cout << "\nInvalid credentials or unregistered user. Please check users.txt" << endl;
    }

    return 0;
}
