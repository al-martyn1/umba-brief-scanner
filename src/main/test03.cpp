/*! \file
    \brief Тест arithmetic_strong_typedef - просто присунул тут по случаю
*/


#include <string>
#include <iostream>

#include "umba/arithmetic_strong_typedef.h"


UMBA_INTEGRAL_ARITHMETIC_STRONG_TYPEDEF( Speed, int )


int main(int argc, char* argv[])
{

    using std::cout;
    using std::endl;
    
    auto speed = Speed(0);

    cout << "Speed: " << speed << endl;

    speed = speed + 3;

    cout << "Speed: " << speed << endl;

    speed += 2;

    cout << "Speed: " << speed << endl;

    speed *= 2;

    cout << "Speed: " << speed << endl;

    speed /= 3;

    cout << "Speed: " << speed << endl;

    speed = speed - 1;

    cout << "Speed: " << speed << endl;

    speed = speed * 20;

    cout << "Speed: " << speed << endl;

    speed %= 7;

    cout << "Speed: " << speed << endl;

    return 0;
}