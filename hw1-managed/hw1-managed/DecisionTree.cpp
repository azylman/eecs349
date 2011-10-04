#include "StdAfx.h"
#include "DecisionTree.h"

void DecisionTree::build(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {
	if (trainingSet->Count() == 0) {
		label = trainingSet->mostCommonClassification();
		Console::WriteLine("trainingSet is empty and label is " + label);
	} else if (trainingSet->allPositive()) {
		label = "1";
	} else if (trainingSet->allNegative()) {
		label = "0";
	} else if (attributes->Count == 0) {
		label = trainingSet->mostCommonClassification();
		Console::WriteLine("attributes is empty and label is " + label);
	} else {
		decisionAttribute = trainingSet->getBestClassifer(attributes);
		Dictionary<String^, List<String^>^>^ newAttributes = gcnew Dictionary<String^, List<String^>^>(attributes);
		List<String^>^ possibleValues = attributes[decisionAttribute];
		newAttributes->Remove(decisionAttribute);
		for each (String^ value in possibleValues) {
			Dictionary<String^, String^>^ decision = gcnew Dictionary<String^, String^>();
			decision->Add(decisionAttribute, value);
			Items^ newTrainingSet = trainingSet->filterListByDecisions(decision);
			children->Add(value, gcnew DecisionTree(newTrainingSet, newAttributes));
		}
	}
}

DecisionTree::DecisionTree(Items^ trainingSet, Items^ testingSet, Dictionary<String^, List<String^>^>^ attributes) : trainingSet(trainingSet), testingSet(testingSet) {
	initValues();
	build(trainingSet, attributes);
}

DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {
	initValues();
	build(trainingSet, attributes);
}

void DecisionTree::initValues() {
	children = gcnew Dictionary<String^, DecisionTree^>();
	decisionAttribute = "";
	label = "";
}

void DecisionTree::print() {
	print(0);
}

void DecisionTree::print(int depth) {
	String^ indent = "";
	for (int i = 0; i < depth; ++i) {
		indent += "  ";
	}
	for each (KeyValuePair<String^, DecisionTree^>^ child in children) {
		Console::WriteLine(indent + decisionAttribute + " = " + child->Key);
		child->Value->print(depth+1);
	}
	if (children->Count == 0) {
		Console::WriteLine(indent + decisionAttribute + label);
	}
}