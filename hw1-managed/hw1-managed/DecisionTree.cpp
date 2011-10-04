#include "StdAfx.h"
#include "DecisionTree.h"

void DecisionTree::build(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions) {
	children = gcnew Dictionary<String^, DecisionTree^>();
	decisionAttribute = "";
	label = "";

	if (trainingSet->Count() == 0) {
		label = trainingSet->mostCommonClassification();
	} else if (trainingSet->allPositive()) {
		label = "+";
	} else if (trainingSet->allNegative()) {
		label = "-";
	} else if (attributes->Count == 0) {
		label = trainingSet->mostCommonClassification();
	} else {
		decisionAttribute = trainingSet->getBestClassifer(decisions);
		Dictionary<String^, List<String^>^>^ newAttributes = gcnew Dictionary<String^, List<String^>^>(attributes);
		List<String^>^ possibleValues = attributes[decisionAttribute];
		newAttributes->Remove(decisionAttribute);
		for each (String^ value in possibleValues) {
			Dictionary<String^, String^>^ newDecisions = gcnew Dictionary<String^, String^>(decisions);
			newDecisions->Add(decisionAttribute, value);
			Items^ newTrainingSet = trainingSet->filterListByDecisions(newDecisions);
			children->Add(value, gcnew DecisionTree(newTrainingSet, newAttributes, newDecisions));
		}
	}
}

DecisionTree::DecisionTree(Items^ trainingSet, Items^ testingSet, Dictionary<String^, List<String^>^>^ attributes) : trainingSet(trainingSet), testingSet(testingSet) {
	build(trainingSet, attributes, gcnew Dictionary<String^, String^>());
}

DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions) {
	build(trainingSet, attributes, decisions);
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