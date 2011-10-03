// hw1-managed.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

int main(array<String^> ^args)
{
	List<String^>^ argsList = gcnew List<String^>(args);
	if (argsList->Count != 4) {
		Console::WriteLine("Error! Must have four arguments: inputFileName, trainingSetSize, numberOfTrials, verbose");
		return -1;
	}
    String^ inputFileName = argsList[0];
	int trainingSetSize = Convert::ToInt32(argsList[1]);
	int numberOfTrials = Convert::ToInt32(argsList[2]);
	bool verbose = Convert::ToBoolean(argsList[3]);

	

    return 0;
}
