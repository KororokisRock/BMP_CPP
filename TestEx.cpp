#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "func.h"


int main()
{
    BMPFile bmpFile;
    bmpFile.readBMPFile(getPathToInputFile()); //C:\Users\Maksim\Desktop\Test\files\test1.bmp
    bmpFile.printImgInConsole();
    bmpFile.createXInImg();
    bmpFile.printImgInConsole();
    bmpFile.createBMPFile(getPathToOutputFile());
    bmpFile.deleteRes();
}

