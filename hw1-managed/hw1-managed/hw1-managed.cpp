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
	List<String^>^ possibleAttributeNames = gcnew List<String^>(dataset->ReadLine()->Split());
	List<String^>^ attributeNames = gcnew List<String^>();
	for each(String^ attribute in possibleAttributeNames) {
		if (!String::IsNullOrEmpty(attribute->Trim())) {
			attributeNames->Add(attribute);
		}
	}
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
			String^ name = attributeNames[i]->Trim();
			String^ value = attributes[i]->Trim();
			attributeDict->Add(name, value);
			createOrAddToAttribute(possibleAttributes, name, value);
		}
		items->Add(gcnew Item(attributeDict));
	}
	items->setAttributes(gcnew Dictionary<String^, List<String^>^>(possibleAttributes));
	
	trainingSetSize = trainingSetSize > items->Count() ? items->Count() : trainingSetSize;

	// We don't want this in our attributes to classify in the decision tree.
	possibleAttributes->Remove("CLASS");
	List<DecisionTree^>^ decisionTrees = gcnew List<DecisionTree^>();
	Items^ trainingSet;
	Items^ testingSet;
	for (int i = 0; i < numberOfTrials; ++i) {
		trainingSet = items->getTrainingSet(trainingSetSize);
		testingSet = items->getTestingSet(trainingSet);
		decisionTrees->Add(gcnew DecisionTree(trainingSet, testingSet, gcnew Dictionary<String^, List<String^>^>(possibleAttributes)));
	}

	double treePassing = 0;
	double priorPassing = 0;
	int numTrees = 1;
	for each (DecisionTree^ tree in decisionTrees) {
		
		Console::WriteLine("");
		Console::WriteLine("Trial #" + numTrees++);
		Console::WriteLine("----------------------------");

		tree->print();

		Console::WriteLine("");

		double trialTreePassing = tree->classifyTestSet();
		Console::WriteLine("% correct from tree: " + trialTreePassing);
		treePassing += trialTreePassing;

		double trialPriorPassing = tree->priorClassifyTestSet();
		Console::WriteLine("% correct from prior probability: " + trialPriorPassing);
		priorPassing += trialPriorPassing;
	}
	Console::WriteLine("\n\n");
	Console::WriteLine("Using dataset " + inputFileName + " with " + numberOfTrials + " trials");
	Console::WriteLine("Test set size: " + testingSet->Count() + ", training set size: " + trainingSet->Count());
	Console::WriteLine("Average correct classification of decision tree: " + treePassing / (double) numberOfTrials);
	Console::WriteLine("Average correct classification of prior probability: " + priorPassing / (double) numberOfTrials);
    return 0;
}