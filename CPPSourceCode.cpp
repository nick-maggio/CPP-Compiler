#include <iostream>
using namespace std;

int main() {
    int a = 1;
    int b = 2;
    double x = 4.5;
    string msg = "hello";

    int c = a + b * 2;
    int d = (a + b) * 2;
    int e = c % 3;

    if (a == b) {
        cout << "equal";
    } else if (a < b) {
        cout << "less";
    } else {
        cout << "greater";
    }

    while (a < 10) {
        a = a + 1;
        if (a == 7) {
            continue;
        }
    }

    do {
        a = a - 1;
    } while (a > 0);

    for (int i = 0; i < 10; i = i + 1) {
        if (i == 5) {
            break;
        }
        cout << i;
    }

    switch (a) {
        case 0:
            cout << "zero";
            break;
        case 1:
            cout << "one";
            break;
        default:
            cout << "other";
            break;
    }

    int neg = -a;
    int isNot = !b;
    a++;
    b--;
    ++a;
    --b;

    cout << a << b << msg;

    return 0;
}