#ifndef CLIKIT_HPP
#define CLIKIT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>

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

inline void print(string text, float time, string colorCode = defaultColor) {
    cout << colorCode;
    for (char c : text) {
        cout << c;
        cout.flush();
        usleep(time * microsecond);
    }
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
    int m_maxVisibleItems;

public:
    Menu(string header, vector<MenuItem<T>> menuItems, float speed = 0.01, int maxVisibleItems = 5) 
        : m_menuItems(menuItems), m_header(header), displaySpeed(speed), m_maxVisibleItems(maxVisibleItems) {}

    void Display(float time) {
        #ifdef _WIN32
            system("cls");
        #elif __linux__
            system("clear");
        #endif
        int startIndex = max(0, selectedItem - (m_maxVisibleItems / 2));
        int endIndex = min(static_cast<int>(m_menuItems.size()), startIndex + m_maxVisibleItems);

        if (startIndex > 0) {
            print("\u2191", time);
        }
        if (endIndex - startIndex < m_maxVisibleItems) {
            startIndex = max(0, endIndex - m_maxVisibleItems);
        }
        for (int i = startIndex; i < endIndex; ++i) {
            if (i == selectedItem) {
                print("-> " + m_menuItems[i].text, time, "\033[1;32m");
            } else {
                print("  " + m_menuItems[i].text, time, m_menuItems[i].color);
            }
        }
        if (endIndex < m_menuItems.size()) {
            print("\u2193", time);
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

#endif // CLIKIT_HPP
