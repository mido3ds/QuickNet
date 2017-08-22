#include <iostream>
#include "QuickNetApp.h"
using namespace std;

int main(int argc, char* argv[])
{
    QuickNetApp("", argv[1], true).Run();
}