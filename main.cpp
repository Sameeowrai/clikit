#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>

using namespace std;
unsigned int microsecond = 1000000;
string defaultColor = "\e[0;37m";
float gameSpeed = 0.01;
#define ARROW_UP    65
#define ARROW_DOWN  66
#define ENTER       10

void enableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

void disableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

void print(string text, float time, string colorCode = defaultColor) {
    cout << colorCode;
    for (char c : text) {
        cout << c;
        cout.flush();
        usleep(time * microsecond);
    }
    cout << endl;
}

class Menu {
    vector<string> m_menuItems;
    string m_header;
    int selectedItem = 0;
    float displaySpeed;

public:
    Menu(string header, vector<string> menuItems, float speed = 0.01) : m_menuItems(menuItems), m_header(header, displaySpeed = speed ) {}

    void Display(float time) {
        #ifdef _WIN32
            system("cls");
        #elif __linux__
            system("clear");
        #endif
        print(m_header, time);
        for (int i = 0; i < m_menuItems.size(); ++i) {
            if (i == selectedItem) {
                print("-> " + m_menuItems[i], time, "\033[1;32m");
            } else {
                print("  " + m_menuItems[i], time);
            }
        }
    }

    int Get() {
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
                return selectedItem;
            }
            Display(0);
        }
    }

    void Insert(string text){
        m_menuItems.push_back(text);
    }
};


int main(){
    print("Hello world",0.1);
    while(1){
        Menu menu("Menu", {"Home","Contact Us","Exit"},0.05);
        int res = menu.Get();
        switch (res)
        {
        case 0:
            print("Welcome Home",0.1,"\e[1;33m");
            break;
        case 1:
            print("-> My github: https://github.com/Sameeowrai\n-> This project repo: https://github.com/Sameeowrai/clikit",0.1,"\e[1;31m");
            break;
        case 2:{
            Menu exitMenu("Are you sure?", {"yes","no"});
            if(exitMenu.Get() == 0){
                exit(0);
            }
        }
        default:
            break;
        }
        cin.ignore();
        usleep(microsecond);
    }
}