// hw1-managed.cpp : main project file.

#include "stdafx.h"
#include "DecisionTree.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

// Creates (if the key doesn't exist) or adds (if the key does) an attribute value to our table of possible values
void createOrAddToAttribute(Dictionary<String^, List<String^>^>^ attributes, String^ attribute, String^ value) {
	
	if (attributes->ContainsKey(attribute)) { // If the attribute already exists in the table...
		// We need to retrieve it and store it in a HashSet to ensure uniqueness
		HashSet<String^>^ attributeValues = gcnew HashSet<String^>(attributes[attribute]);
		attributeValues->Add(value);
		// And convert it back to a list when we store it back in after adding the value
		attributes[attribute] = gcnew List<String^>(attributeValues);
	} else { // If it doesn't exist, just create a list with the value and add it to the table
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

	// Create the possible attribute namnes
	List<String^>^ possibleAttributeNames = gcnew List<String^>(dataset->ReadLine()->Split());

	// possibleAttributeNames will have empty strings as well so we need to filter those out
	List<String^>^ attributeNames = gcnew List<String^>();
	for each(String^ attribute in possibleAttributeNames) {
		if (!String::IsNullOrEmpty(attribute->Trim())) {
			attributeNames->Add(attribute);
		}
	}

	// A table from the attribute name to the list of possible attribute values
	Dictionary<String^, List<String^>^>^ possibleAttributes = gcnew Dictionary<String^, List<String^>^>();

	Items^ items = gcnew Items();
	// Read in all the items from the file
	while (String^ item = dataset->ReadLine()) {

		// Read the item's attributes out (splitting on tab characters, the delimiter between attributes
		List<String^>^ attributes = gcnew List<String^>(item->Split('\t'));

		// The dictionary holding all of the attributes for the item (attributeName -> attributeValue)
		Dictionary<String^, String^>^ attributeDict = gcnew Dictionary<String^, String^>();

		// There should always be the same number of attributes read as attribute names
		if (attributes->Count != attributeNames->Count) {
			Console::WriteLine("Malformed dataset - attribute size is not equal to the number of attribute names");
			return -1;
		}

		// Put our attributes in the dictionary
		for (int i = 0; i < attributes->Count; ++i) {
			String^ name = attributeNames[i]->Trim();
			String^ value = attributes[i]->Trim();
			attributeDict->Add(name, value);

			// Also keep track of all of the possible values for any attribute
			createOrAddToAttribute(possibleAttributes, name, value);
		}

		// Create an item with the given attributes
		items->Add(gcnew Item(attributeDict));
	}
	
	// If the size of the training set is larger than the amount of items in the data set, change it
	trainingSetSize = trainingSetSize > items->Count() ? items->Count() : trainingSetSize;

	// We don't want this in our attributes to branch based on in the decision tree
	possibleAttributes->Remove("CLASS");

	List<DecisionTree^>^ decisionTrees = gcnew List<DecisionTree^>();
	Items^ trainingSet;
	Items^ testingSet;
	// Create as many decision trees as we have trials
	for (int i = 0; i < numberOfTrials; ++i) {
		trainingSet = items->getTrainingSet(trainingSetSize);
		testingSet = items->getTestingSet(trainingSet);

		DecisionTree^ tree = gcnew DecisionTree(trainingSet, testingSet, gcnew Dictionary<String^, List<String^>^>(possibleAttributes));
		tree->setVerbosePrinting(verbose);

		decisionTrees->Add(tree);
	}

	double treePassing = 0;
	double priorPassing = 0;
	int numTrees = 1;

	// Print out all of our learned trees
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

	// Print out the more general statistics
	Console::WriteLine("\n\n");
	Console::WriteLine("Using dataset " + inputFileName + " with " + numberOfTrials + " trials");
	Console::WriteLine("Test set size: " + testingSet->Count() + ", training set size: " + trainingSet->Count());
	Console::WriteLine("Average correct classification of decision tree: " + treePassing / (double) numberOfTrials);
	Console::WriteLine("Average correct classification of prior probability: " + priorPassing / (double) numberOfTrials);
    return 0;
}