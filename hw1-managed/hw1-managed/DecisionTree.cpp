#include "StdAfx.h"
#include "DecisionTree.h"


DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions)
		: trainingSet(trainingSet), attributes(attributes), decisions(decisions) {

	if (trainingSet->filterListByDecisions(decisions)->Count == 0) {
		label = trainingSet->mostCommonClassification();
	} else if (trainingSet->allPositive()) {
		label = "+";
	} else if (trainingSet->allNegative()) {
		label = "-";
	} else if (attributes->Count == 0) {
		label = trainingSet->mostCommonClassification();
	} else {
		children = gcnew Dictionary<String^, DecisionTree^>();
		decisionAttribute = trainingSet->getBestClassifer(decisions);
		Dictionary<String^, List<String^>^>^ newAttributes = gcnew Dictionary<String^, List<String^>^>(attributes);
		newAttributes->Remove(decisionAttribute);
		for each (String^ value in attributes[decisionAttribute]) {
			Dictionary<String^, String^>^ newDecisions = gcnew Dictionary<String^, String^>(decisions);
			newDecisions->Add(decisionAttribute, value);
			children->Add(value, gcnew DecisionTree(trainingSet, newAttributes, newDecisions));
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
		indent += " ";
	}
	for each (KeyValuePair<String^, DecisionTree^>^ child in children) {
		Console::WriteLine(indent + decisionAttribute + " = " + child->Key);
		child->Value->print(depth+1);
	}
}