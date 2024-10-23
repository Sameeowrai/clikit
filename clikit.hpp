#ifndef CLIKIT_HPP
#define CLIKIT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <sstream>

const unsigned int microsecond = 1000000;
using namespace std;

const string defaultColor = "\e[0;37m";
const float gameSpeed = 0.01;
#define ARROW_UP    65
#define ARROW_DOWN  66
#define ENTER       10

// Function declarations
inline void enableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

inline void disableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

inline void print(string text, float time, string colorCode = defaultColor, bool endline = 1) {
    cout << colorCode;
    for (char c : text) {
        cout << c;
        cout.flush();
        usleep(time * microsecond);
    }
    if(endline == 1)
        cout << endl;
}

// MenuItem struct
template <typename T>
struct MenuItem {
    string text;
    string color;
    T value;
};

// Menu class
template <typename T>
class Menu {
    vector<MenuItem<T>> m_menuItems;
    string m_header;
    int selectedItem = 0;
    float displaySpeed;

public:
    Menu(string header, vector<MenuItem<T>> menuItems, float speed = 0.01) 
        : m_menuItems(menuItems), m_header(header), displaySpeed(speed) {}

    void Display(float time) {
        #ifdef _WIN32
            system("cls");
        #elif __linux__
            system("clear");
        #endif
        print(m_header, time);
        for (int i = 0; i < m_menuItems.size(); ++i) {
            if (i == selectedItem) {
                print("-> " + m_menuItems[i].text, time, "\033[1;32m");
            } else {
                print("  " + m_menuItems[i].text, time, m_menuItems[i].color);
            }
        }
    }

    T Get() {
        enableRawMode();
        char c;
        Display(displaySpeed);
        while (true) {
            read(STDIN_FILENO, &c, 1);
            if (c == 27) { // Escape sequence
                char buf[3];
                if (read(STDIN_FILENO, buf, 2) == 2) {
                    if (buf[1] == ARROW_UP && selectedItem > 0) selectedItem--;
                    else if (buf[1] == ARROW_DOWN && selectedItem < m_menuItems.size() - 1) selectedItem++;
                }
            } else if (c == ENTER) {
                disableRawMode();
                return m_menuItems[selectedItem].value;
            }
            Display(0);
        }
    }

    void Insert(MenuItem<T> item) {
        m_menuItems.push_back(item);
    }
};

// Input class
template <typename T>
class Input{
private:
    string m_text;
    float speed;
    string m_color;
public:
    Input(string text, float time, string color = defaultColor): m_text(text),speed(time),m_color(color){}
    
    T Read(){
        print(m_text,speed,m_color,0);
        T result;
        cin >> result;
        return result;
    }

    vector<T> ReadByDelimiter(const string& delimiter) {
        print(m_text, speed, m_color,0);
        string input;
        getline(cin, input);

        vector<T> results;
        stringstream ss(input);
        string item;

        while (getline(ss, item, delimiter[0])) {
            stringstream itemStream(item);
            T value;
            if (itemStream >> value) { 
                results.push_back(value);
            }
        }
        
        return results;
    }

    

};

#endif // CLIKIT_HPP
