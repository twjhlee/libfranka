
#include <iostream>
#include <termios.h>

#define STDIN_FILENO 0
using namespace std;
int main()
{
    // Black magic to prevent Linux from buffering keystrokes.
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    while(1){
    // Once the buffering is turned off, the rest is simple.
        char c,d,e;
        cin >> c;
        cin >> d;
        cin >> e;
        //cout << "\nYour character was ";
    // Using 3 char type, Cause up down right left consist with 3 character
        if ((c==27)&&(d=91)) {
            cout << c << " " << d << endl;
            //if (e==65) { cout << "UP" << endl;}
            //if (e==66) { cout << "DOWN" << endl;}
            //if (e==67) { cout << "RIGHT" << endl;}
            //if (e==68) { cout << "LEFT" << endl;}
        }
    }

    return 0;
}