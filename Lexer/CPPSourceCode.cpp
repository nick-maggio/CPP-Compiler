#include <iostream>
using namespace std;


int main() {
    // Identifier and declaration tests
    int a = 1;
    int b2 = 2, c_3 = 3;
    double x = 4.25;
    double y = .5;
    double z = 5.;
    a++;
    b2--;
    string msg = "Hello\\nTab\\tQuote:\\\" Backslash:\\\\";   // Heavy testing of string literal escape handling
    string empty = "";

    // Arithmetic operator tests
    a = a + b2;
    a = a - 1;
    a = a * 2;
    a = a / 3;
    a = a % 2;

    // Comparison and unary operator tests
    if (a == b2) {
        cout << "equal";
    } else {
        cout << "not equal";
    }

    if (a != b2) {
        cout << "!=";
    }

    if (!(a < b2)) {
        cout << "< and !";
    }

    if (a <= b2) {
        cout << "<=";
    }

    if (a > b2) {
        cout << ">";
    }

    if (a >= b2) {
        cout << ">=";
    }

    // Input / output operator tests
    cin >> a >> b2;
    cout << msg << a << b2 << x << y << z;

    // while test
    while (a < 10) {
        a = a + 1;
        if (a == 7) {
            continue;
        }
    }

    // do-while test
    do {
        a = a - 1;
    } while (a > 0);

    // for test
    for (int i = 0, j = 3; i < j; i = i + 1) {
        cout << i;
        if (i == 1) {
            break;
        }
    }

    // switch / case / default / colon test
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

    // Nested blocks and parens
    {
        int inner_value = (a + b2) * (c_3 - 1);
        cout << inner_value;
    }

    // Return test
    return 0;
}

// Single-line comment with tokens that should be ignored:
// if else while do for switch case default break continue cin cout return == != <= >= << >> , : { } ( ) ;

// Multi-line comment testing ignored tokens:
/*
int fake = 999;
string fakeMsg = "this should not tokenize as code tokens outside comments";
if (fake >= 0) {
    cout << fakeMsg;
}
*/

// Invalid tokens below: Uncomment them to test lexer's handling of invalid/unknown tokens:
/*

$
#junk
@

*/

//End of file