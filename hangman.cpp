#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

// Define color escape sequences
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define ORANGE  "\033[38;5;208m"

//Khởi tạo các biến toàn cục
string Word_to_guess;
string Word_guessed;
int Wrong_counter;
int Guess_counter;

void capitalize(string &word){
    for (int i = 0; i < word.length(); i++){
        if (word[i] <= 'z' && word[i] >= 'a') word[i] += 'A' - 'a';
    }
}

void printMsg(string message, const bool &printTop = true, const bool &printBottom = true, const string color = RESET){
    //In nửa trên
    cout << RESET;
    if (printTop){
        cout << "+--------------------------------+" << endl;
    }
    cout << "|";

    //Đưa message có độ dài hợp với giao diện và in ra
    bool front = true;
    bool printHighscore = ("Hangman" == message);
    cout << color;
    for (int i = message.length(); i < 32; i++){
        if (front) message = " " + message;
        else message = message + " ";
        front = !front;
    }
    cout << message;
    cout << RESET;

    //In nửa dưới
    cout << "|";
    if (printHighscore){
        fstream highscore("resource\\highscore.txt", ios::in);
        if (!highscore){
            cerr << "Error opening highscore" << endl;
            system("pause");
        }
        string bestWord;
        string bestGuess;
        getline(highscore, bestWord);
        highscore >> bestGuess;
        cout << "  " << YELLOW << "Personal best: " + bestWord + ", " + bestGuess + " guess time" << RESET;
        highscore.close(); 
    }
    cout << endl;
    if (printBottom){
        cout << "+--------------------------------+" << endl;
    }
}

void drawHanged(int counter = 0){
    printMsg("|", 0, 0, ORANGE);
    printMsg("|", 0, 0, ORANGE);
    if (counter >= 1) printMsg("O", 0, 0, RED);
    else printMsg("", 0, 0);
    if (counter == 2) printMsg("/  ", 0, 0, RED);
    if (counter == 3) printMsg("/| ", 0, 0, RED);
    if (counter >= 4) printMsg("/|\\", 0, 0, RED);
    else printMsg("", 0, 0);
    if (counter == 5) printMsg("/  ", 0, 0, RED);
    if (counter == 6) printMsg("/ \\", 0, 0, RED);
    else printMsg("", 0, 0);
    printMsg("", 0, 0);
    printMsg("+---------+", 0, 0, ORANGE);
    printMsg("|         |", 0, 0, ORANGE);
}

void printLetter(string input, char from, char to){
    string Letters ="";
    for (char i = from; i <= to; i++){
        if (input.find(i) == string::npos){
            Letters = Letters + i + " ";
        } else {
            Letters += " ";
        }
    }
    printMsg(Letters, 0, 0, GREEN);
}

void printAvailableLetters(string chosen = ""){
    printMsg("Available Letters", 1, 1);
    printLetter(chosen, 'A', 'M');
    printLetter(chosen, 'N', 'Z');
}

bool printWordAndCheckWin(){
    bool won = true;
    string s = "";
    for (int i = 0; i < Word_to_guess.length(); i++){
        if (Word_guessed.find(Word_to_guess[i]) == string::npos){
            won = false;
            s += "_ ";
        } else {
            s += Word_to_guess[i];
            s += " ";
        }
    }

    char temp = Word_guessed.back();
    bool found = false;
    for (int i = 0; i < Word_to_guess.length(); i++){
        if (temp == Word_to_guess[i]){
            found = true;
            break;
        }
    }
    if (Guess_counter && !found) Wrong_counter++;

    if (Wrong_counter == 6){
        system("cls");
        printMsg("Hangman", 1, 1, YELLOW);
        drawHanged(Wrong_counter);
        printAvailableLetters(Word_guessed);
        printMsg(s, 1, 1);
        string time_guessed = to_string(Guess_counter);
        printMsg("You died!", 0, 0, RED);
        printMsg("Time guessed: " + time_guessed + ".", 0, 0, RED);
        printMsg("The word is " + Word_to_guess + ".", 0, 1, RED);
        return true;
    }
    else if (won == false){
        system("cls");
        printMsg("Hangman", 1, 1, YELLOW);
        drawHanged(Wrong_counter);
        printAvailableLetters(Word_guessed);
        printMsg(s, 1, 1);
        printMsg("Guess the word", 0, 1, CYAN);
    }
    else {
        system("cls");

        //Lưu highscore
        string bestGuess;
        fstream highscore("resource\\highscore.txt", ios::in);
        if (!highscore) {
            cerr << "Error opening highscore file" << endl;
            system("pause");
        }
        getline(highscore, bestGuess);
        highscore.close();
        if (stoi(bestGuess) > Guess_counter){
            fstream highscore("resource\\highscore.txt", ios::out);
            if (!highscore) {
                cerr << "Error opening highscore file" << endl;
                system("pause");
            }
            highscore << "" + Word_to_guess + "\n" + to_string(Guess_counter);
            highscore.close();
        }

        printMsg("Hangman", 1, 1, YELLOW);
        drawHanged(Wrong_counter);
        printAvailableLetters(Word_guessed);
        printMsg(s, 1, 1);
        string time_guessed = to_string(Guess_counter);
        printMsg("You won!", 0, 0, YELLOW);
        printMsg("Time guessed: " + time_guessed + ".", 0, 1, YELLOW);
    }
    return won;
}

string loadRandomWord(string path){
    string word;
    vector<string> v;
    ifstream file(path);
    if (file.is_open()){
        while(getline(file, word)) v.push_back(word);
        srand(time(0));
        int randomLine = rand() % v.size();
        word = v[randomLine];
        file.close();
    } else {
        cout << RED << "Error: Unable to open word file." << RESET << endl;
        system("pause");
        exit(0);
    };
    return word;
}

string choose_Word_Lib_And_Get_A_Random_Word(){
    int libnum;
    printMsg("Choose one word library", 1, 0, CYAN);
    printMsg("1. All          ", 0, 0, GREEN);
    printMsg("2. Asia         ", 0, 0, GREEN);
    printMsg("3. Fruits       ", 0, 0, GREEN);
    printMsg("4. Jobs         ", 0, 0, GREEN);
    printMsg("5. Plants       ", 0, 0, GREEN);
    printMsg("Input word library number:", 1, 1, CYAN);
    cin >> libnum;
    switch (libnum){
        case 1: return loadRandomWord("resource\\words\\all.txt");
        case 2: return loadRandomWord("resource\\words\\asia.txt");
        case 3: return loadRandomWord("resource\\words\\fruits.txt");
        case 4: return loadRandomWord("resource\\words\\jobs.txt");
        case 5: return loadRandomWord("resource\\words\\plants.txt");
        default: return loadRandomWord("resource\\words\\all.txt");
    }
}

void RunGame(){
    printWordAndCheckWin();
    do {
        string guessed;
        cin >> guessed;
        capitalize(guessed);
        Word_guessed += guessed;
        Guess_counter++;
    } while (!printWordAndCheckWin());
}

void GameStart(){
    //Choosing word menu
    system("cls");
    printMsg("Hangman", 1, 1, YELLOW);
    Word_to_guess = choose_Word_Lib_And_Get_A_Random_Word();
    capitalize(Word_to_guess);
    system("cls");

    //Gamestart menu
    printMsg("Hangman", 1, 1, YELLOW);
    drawHanged();
    printAvailableLetters();
}

bool GameEnd(){
    printMsg("Press 0 to replay", 0, 0, GREEN);
    printMsg("Press any other keys to exit", 0, 1, GREEN);
    int userInput;
    cin >> userInput;
    return userInput;
}

int main(){
    starting_point:
    Word_to_guess = "";
    Word_guessed = "";
    Wrong_counter = 0;
    Guess_counter = 0;
    GameStart();
    RunGame();
    if (!GameEnd()) goto starting_point;
    return 0;
}
