
#include <swilib.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char** argv)
{
    ofstream file;
    file.open ("4:\\elf\\c++\\log.txt");

    string buf = "Hello";
    file << buf << endl;
    file.close();


    ShowMSG(1, (int)"O-o");
    kill_elf();
    return 0;
}

/*
-fno-enforce-eh-specs
-fno-rtti
-fno-use-cxa-get-exception-ptr
-fno-non-call-exceptions
-fno-exceptions
-fno-use-cxa-atexit

*/
