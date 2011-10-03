// hw1-managed.cpp : main project file.

#include "stdafx.h"
#include "Items.h"

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

	// Initialize our parameters
    String^ inputFileName = argsList[0];
	int trainingSetSize = Convert::ToInt32(argsList[1]);
	int numberOfTrials = Convert::ToInt32(argsList[2]);
	bool verbose = Convert::ToBoolean(argsList[3]);

	// Read in the dataset
	StreamReader^ dataset = gcnew StreamReader(inputFileName);

	// Create our items list from our input
	List<String^>^ attributeNames = gcnew List<String^>(dataset->ReadLine()->Split('\t'));
	Items^ items = gcnew Items();
	while (String^ item = dataset->ReadLine()) {
		List<String^>^ attributes = gcnew List<String^>(item->Split('\t'));
		Dictionary<String^, String^>^ attributeDict = gcnew Dictionary<String^, String^>();
		if (attributes->Count != attributeNames->Count) {
			Console::WriteLine("Malformed dataset - attribute size is not equal to the number of attribute names");
			return -1;
		}
		for (int i = 0; i < attributes->Count; ++i) {
			attributeDict->Add(attributeNames[i], attributes[i]);
		}
		items->Add(gcnew Item(attributeDict));
	}

    return 0;
}
