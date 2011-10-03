#include "StdAfx.h"
#include "DecisionTree.h"


DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions)
		: trainingSet(trainingSet), attributes(attributes), decisions(decisions) {

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
		newAttributes->Remove(decisionAttribute);
		for each (String^ value in attributes[decisionAttribute]) {
			Dictionary<String^, String^>^ newDecisions = gcnew Dictionary<String^, String^>(decisions);
			Items^ newTrainingSet = trainingSet->filterListByDecisions(newDecisions);
			newDecisions->Add(decisionAttribute, value);
			children->Add(value, gcnew DecisionTree(newTrainingSet, newAttributes, newDecisions));
		}
	}
}

DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {
	DecisionTree(trainingSet, attributes, gcnew Dictionary<String^, String^>());
}

void DecisionTree::print() {
	print(0);
}

void DecisionTree::print(int depth) {
	String^ indent = "";
	for (int i = 0; i < depth; ++i) {
		indent += "\t";
	}
	for each (KeyValuePair<String^, DecisionTree^>^ child in children) {
		Console::WriteLine(indent + decisionAttribute + " = " + child->Key);
		child->Value->print(depth+1);
	}
	if (children->Count == 0) {
		Console::WriteLine(indent + decisionAttribute + label);
	}
}