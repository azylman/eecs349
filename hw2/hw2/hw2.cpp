// hw2.cpp : main project file.

#include "stdafx.h"
#include "Dict.h"

using namespace System;

int main(array<System::String ^> ^args)
{	
	String^ path = "../";
	// Create our file paths
    String^ dictPath = path + "3esl.txt";

	Dict^ dict = gcnew Dict(dictPath);

	Console::WriteLine(dict->getCorrectWord("ablyl"));

	Console::WriteLine(dict->getCorrectWord("ablyl"));

    Console::WriteLine(L"Hello World");
    return 0;
}
