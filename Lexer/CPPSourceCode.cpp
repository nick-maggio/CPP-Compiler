//This is the file that is being compiled.
#include <iostream>
using namespace std;

int main()
{

    //Single-line comment: This is a single-line comment

    /*      
    This is a multi-line comment
    */

    int x = 5;
    int y = 10;
    double z = 5.33;

    string iostream = "This is a string literal";

    if (x < y) {
        cout << iostream;
    } else {
        cout << "x is not less than y";
    }
    
    cout << z;

    return 0;
}