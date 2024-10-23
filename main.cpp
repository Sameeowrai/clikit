#include <iostream>
#include <unistd.h>

unsigned int microsecond = 1000000;
using namespace std;

string defaultColor = "\e[0;37m";


void print(string text, float time, string colorCode = defaultColor) {
    cout << colorCode;
    for (char c : text) {
        cout << c;
        cout.flush();
        usleep(time * microsecond);
    }
    cout << endl;
}

int main(){
    print("Hello world",0.1);
}