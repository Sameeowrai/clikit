#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <variant>


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

template <typename T>
struct MenuItem{
    string text;
    string color;
    T value;
};

template <typename T>
class Menu {
    vector<MenuItem<T>> m_menuItems;
    string m_header;
    int selectedItem = 0;
    float displaySpeed;

public:
    Menu(string header, vector<MenuItem<T>> menuItems, float speed = 0.01) : m_menuItems(menuItems), m_header(header), displaySpeed(speed)  {}

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
                print("  " + m_menuItems[i].text, time,m_menuItems[i].color);
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

    void Insert(MenuItem<T> item){
        m_menuItems.push_back(item);
    }

};


int main(){
    print("Hello world",0.1);
    while(1){
        Menu<int> menu("Menu", {
            {"Home","\e[1;35m", 0},
            {"Contact Us","\e[1;34m", 1},
            {"Exit","\e[1;31m", 2}
        },0.05);
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
            Menu<int> exitMenu("Are you sure?", {
                {"yes", "\e[0;37m", 0},
                {"no", "\e[0;37m", 1}
            });
            if(exitMenu.Get() == 0){
                exit(0);
            }
        }
        default:
            break;
        }
    }
}
