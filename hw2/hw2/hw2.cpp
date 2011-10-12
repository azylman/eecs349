// hw2.cpp : main project file.

#include "stdafx.h"
#include "Dict.h"

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;

int main(array<System::String ^> ^args)
{	
	String^ path = "../";
	// Create our file paths
    String^ dictPath = path + "3esl.txt";
	String^ testingPath = path + "wikipediatypoclean.txt";

	Dict^ dict = gcnew Dict(dictPath);
	Dictionary<String^, String^>^ testingData = gcnew Dictionary<String^, String^>();

	StreamReader^ testingDataFile = gcnew StreamReader(testingPath);

	while (String^ data = testingDataFile->ReadLine()) {
		List<String^>^ parts = gcnew List<String^>(data->Split('\t'));
		testingData->Add(parts[0], parts[1]);
	}

	for each (KeyValuePair<String^, String^>^ testData in testingData) {
		Stopwatch^ timer = Stopwatch::StartNew();
		String^ result = dict->getCorrectWord(testData->Key);
		long time = timer->ElapsedMilliseconds;
		Console::Write(testData->Key + " took " + time + " milliseconds to evaluate to " + result + " ");
		if (result == testData->Value) {
			Console::WriteLine("which is CORRECT!");
		} else {
			Console::WriteLine("but it should be " + testData->Value);
		}
	}

    Console::WriteLine(L"Hello World");
    return 0;
}
