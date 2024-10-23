#include <iostream>
#include "clikit.hpp"

int main(){
    Input<int> input("Enter number\n:",0.1);
    vector<int> numbers = input.ReadByDelimiter(" ");
    for(int i: numbers){
        cout << i << endl;
    }

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
