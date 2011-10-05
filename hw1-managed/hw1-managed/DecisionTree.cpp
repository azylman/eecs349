#include "StdAfx.h"
#include "DecisionTree.h"

// Builds a decision tree based on the ID3 algorithm
void DecisionTree::build(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {

	if (trainingSet->allPositive()) { // If the entirety of our training set is positive
		// Stop branching and return true
		label = "true";
	} else if (trainingSet->allNegative()) { // If the entirety of our training set is negative
		// Stop branching and return false
		label = "false";
	} else if (attributes->Count == 0) { // If we have no more attributes to classify based on
		// Stop branching and retun the most common classification from the remaining attributes
		label = trainingSet->mostCommonClassification();
	} else {
		// Determine which attribute we're going to branch on
		decisionAttribute = trainingSet->getBestClassifer(attributes);

		// Get the list of values it's possible for our decision attribute to take
		List<String^>^ possibleValues = attributes[decisionAttribute];

		// Create a new dictionary of attributes available to branch on that is a copy of the current one, but with the decision attribute removed
		Dictionary<String^, List<String^>^>^ newAttributes = gcnew Dictionary<String^, List<String^>^>(attributes);
		newAttributes->Remove(decisionAttribute);

		// Branch at all the possible values
		for each (String^ value in possibleValues) {
			// Create a decision dictionary to encapsulate the decision we just made
			Dictionary<String^, String^>^ decision = gcnew Dictionary<String^, String^>();
			decision->Add(decisionAttribute, value);

			// Create a new training set that is our old one filtered by our new decision
			Items^ newTrainingSet = trainingSet->filterListByDecisions(decision);

			DecisionTree^ child;
			if (newTrainingSet->Count() == 0) { // If there are no items remaining after the decision we made
				// Use the most common classification from our current items
				child = gcnew DecisionTree(trainingSet->mostCommonClassification());
			} else {
				// Otherwise, recur
				child = gcnew DecisionTree(newTrainingSet, newAttributes);
			}
			children->Add(value, child);
		}
	}
}

// Constructor used when initially creating a decision tree
DecisionTree::DecisionTree(Items^ trainingSet, Items^ testingSet, Dictionary<String^, List<String^>^>^ attributes) : trainingSet(trainingSet), testingSet(testingSet) {
	initValues();

	// Calculate the prior probability and store it
	int good = 0;
	int bad = 0;
	for each (Item^ item in trainingSet->getItems()) {
		if (item->isPositive()) {
			good++;
		} else {
			bad++;
		}
	}
	priorProbability = (double) good / (double) (good + bad);

	// Begin building the tree
	build(trainingSet, attributes);
}

// Constructor used when you're recurring from inside build
DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {
	initValues();
	build(trainingSet, attributes);
}

// Constructor used when you need to create a child with a specific label
// This is used in the case in build where the filtered training set is empty
DecisionTree::DecisionTree(String^ label) : label(label) {
	initValues();
}

void DecisionTree::initValues() {
	children = gcnew Dictionary<String^, DecisionTree^>();
	decisionAttribute = "";
	label = "";
}

void DecisionTree::print() {
	if (verbose) {
		Console::WriteLine("Training set:");
		Console::WriteLine(trainingSet->ToString());
		Console::WriteLine("");

		Console::WriteLine("Testing set:");
		Console::WriteLine(testingSet->ToString());
		Console::WriteLine("");
	}
	print(verbose, 0);
}

void DecisionTree::print(bool verbose, int depth) {
	String^ indent = "";
	for (int i = 0; i < depth; ++i) {
		indent += "  ";
	}
	for each (KeyValuePair<String^, DecisionTree^>^ child in children) {
		Console::WriteLine(indent + decisionAttribute + " = " + child->Key);
		child->Value->print(verbose, depth+1);
	}
	if (children->Count == 0) {
		Console::WriteLine(indent + decisionAttribute + label);
	}
}

// Classify a specific item and return of it was classified correctly
bool DecisionTree::classify(Item^ item) {
	if (children->Count > 0) { // If there are more branches
		// Retrieve our item's attribute value for the decision attribute
		String^ attribute = item->GetAttribute(decisionAttribute);
		// And recur
		return children[attribute]->classify(item);
	} else { // If you're at a leaf node (no more branches)
		// Compare the classification to the known value
		return Item::valueIsPositive(label) == item->isPositive();
	}
}

// Classify a set of items and return the proprotion that were correctly classified
double DecisionTree::classify(Items^ dataset) {
	int correct = 0;
	int incorrect = 0;

	for each (Item^ item in dataset->getItems()) {
		if (classify(item)) {
			correct++;
			if (verbose) {
				Console::WriteLine("Tree classified " + item->ToString() + " as " + item->isPositive() + " (correct)");
			}
		} else {
			incorrect++;
			if (verbose) {
				Console::WriteLine("Tree classified " + item->ToString() + " as " + !item->isPositive() + " (incorrect)");
			}
		}
	}

	return (double) correct / (double) (correct + incorrect);
}

// Classify our test set
double DecisionTree::classifyTestSet() {
	if (testingSet->Count() > 0) {
		return classify(testingSet);
	} else {
		return -1;
	}
}

// Classify a set of items based on prior probability
double DecisionTree::priorClassify(Items^ dataset) {
	int correct = 0;
	int incorrect = 0;

	Random^ rnd = gcnew Random();
	for each (Item^ item in dataset->getItems()) {

		// Generate a random number between 0 and 1
		double num = rnd->NextDouble();

		// If this is less than our prior probability, we're classifying this item as positive
		bool classifiedAsPositive = priorProbability > num;

		if (classifiedAsPositive == item->isPositive()) { // If we classify as positive and the item is actually positive
			correct++;
			if (verbose) {
				Console::WriteLine("Prior classified " + item->ToString() + " as " + item->isPositive() + " (correct)");
			}
		} else {
			incorrect++;
			if (verbose) {
				Console::WriteLine("Prior classified " + item->ToString() + " as " + !item->isPositive() + " (incorrect)");
			}
		}
	}

	return (double) correct / (double) (correct + incorrect);
}

// Classify our test set based on our prior probability
double DecisionTree::priorClassifyTestSet() {
	return priorClassify(testingSet);
}

void DecisionTree::setVerbosePrinting(bool verbose) {
	this->verbose = verbose;
}