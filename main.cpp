#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Question {
private:
    string questionText;
    string options[4];
    int correctOption;

public:
    
   Question(string q, string o1, string o2, string o3, string o4, int correct) {
        questionText = q;
        options[0] = o1;
        options[1] = o2;
        options[2] = o3;
        options[3] = o4;
        correctOption = correct;
    }

    
    bool askQuestion() {
        cout << "\n" << questionText << endl;
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ". " << options[i] << endl;
        }
        int choice;
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        return (choice == correctOption);
    }
};

class Quiz {
private:
    vector<Question> questions;
    int score;
    string playerName;

public:
    Quiz(string name) {
        playerName = name;
        score = 0;
    }

    void addQuestion(Question q) {
        questions.push_back(q);
    }

    void startQuiz() {
        cout << "\n--- Welcome " << playerName << " to the C++ Knowledge Quiz! ---\n";
        for (int i = 0; i < questions.size(); i++) {
            cout << "\nQuestion " << i + 1 << ":";
            if (questions[i].askQuestion()) {
                cout << "Correct!\n";
                score += 10;
            } else {
                cout << "Wrong!\n";
            }
        }
        cout << "\nQuiz Finished! Your total score is: " << score << "/" << (questions.size() * 10) << endl;
        saveScore();
    }

    
    void saveScore() {
        ofstream file("scores.txt", ios::app);
        if (file.is_open()) {
            file << "Player: " << playerName << " | Score: " << score << endl;
            file.close();
            cout << "Your score has been successfully saved to scores.txt!" << endl;
        } else {
            cout << "Error opening file to save score." << endl;
        }
    }
};

int main() {
    string name;
    cout << "Enter your name to start the quiz: ";
    getline(cin, name);

    Quiz myQuiz(name);

  
    myQuiz.addQuestion(Question("What does AI stand for?", "Artificial Intelligence", "Automated Interface", "Advanced Informatics", "All of the above", 1));
    myQuiz.addQuestion(Question("Which programming language is most widely used for Machine Learning?", "HTML", "C++", "Python", "CSS", 3));
    myQuiz.addQuestion(Question("What is the time complexity of searching in a perfectly balanced binary search tree?", "O(n)", "O(log n)", "O(n^2)", "O(1)", 2));
    myQuiz.addQuestion(Question("Which data structure uses LIFO (Last In First Out)?", "Queue", "Array", "Tree", "Stack", 4));

    myQuiz.startQuiz();

    return 0;
}
