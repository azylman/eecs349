// hw1-managed.cpp : main project file.

#include "stdafx.h"
#include "DecisionTree.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

void createOrAddToAttribute(Dictionary<String^, List<String^>^>^ attributes, String^ attribute, String^ value) {
	if (attributes->ContainsKey(attribute)) {
		HashSet<String^>^ attributeValues = gcnew HashSet<String^>(attributes[attribute]);
		attributeValues->Add(value);
		attributes[attribute] = gcnew List<String^>(attributeValues);
	} else {
		List<String^>^ list = gcnew List<String^>();
		list->Add(value);
		attributes->Add(attribute, list);
	}
}

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
	bool verbose = argsList[3]->Equals("1") ? true : false;

	// Read in the dataset
	StreamReader^ dataset = gcnew StreamReader(inputFileName);

	// Create our items list from our input
	List<String^>^ attributeNames = gcnew List<String^>(dataset->ReadLine()->Split('\t'));
	Dictionary<String^, List<String^>^>^ possibleAttributes = gcnew Dictionary<String^, List<String^>^>();
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
			createOrAddToAttribute(possibleAttributes, attributeNames[i], attributes[i]);
		}
		items->Add(gcnew Item(attributeDict));
	}
	items->setAttributes(gcnew Dictionary<String^, List<String^>^>(possibleAttributes));
	
	// We don't want this in our attributes to classify in the decision tree.
	possibleAttributes->Remove("CLASS");
	List<DecisionTree^>^ decisionTrees = gcnew List<DecisionTree^>();
	List<Items^>^ trainingSet = gcnew List<Items^>();
	List<Items^>^ testingSet = gcnew List<Items^>();
	for (int i = 0; i < trainingSetSize; ++i) {
		trainingSet->Add(items->getTrainingSet(trainingSetSize));
		testingSet->Add(items->getTestingSet(trainingSet[i]));
		decisionTrees->Add(gcnew DecisionTree(trainingSet[i], possibleAttributes));
	}

	for each (DecisionTree^ tree in decisionTrees) {
		tree->print();
	}

    return 0;
}