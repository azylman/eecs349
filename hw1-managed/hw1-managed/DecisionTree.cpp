#include "StdAfx.h"
#include "DecisionTree.h"


DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions)
		: trainingSet(trainingSet), attributes(attributes), decisions(decisions) {

	printDecisions();
	if (trainingSet->filterListByDecisions(decisions)->Count == 0) {
		label = trainingSet->mostCommonClassification();
	} else if (trainingSet->allPositive()) {
		label = "+";
	} else if (trainingSet->allNegative()) {
		label = "-";
	} else if (attributes->Count == 0) {
		label = trainingSet->mostCommonClassification();
	} else {
		children = gcnew List<DecisionTree^>();
		decisionAttribute = trainingSet->getBestClassifer(decisions);
		Dictionary<String^, List<String^>^>^ newAttributes = gcnew Dictionary<String^, List<String^>^>(attributes);
		newAttributes->Remove(decisionAttribute);
		for each (String^ value in attributes[decisionAttribute]) {
			Dictionary<String^, String^>^ newDecisions = gcnew Dictionary<String^, String^>(decisions);
			newDecisions->Add(decisionAttribute, value);
			children->Add(gcnew DecisionTree(trainingSet, newAttributes, newDecisions));
		}
	}
}

DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {
	DecisionTree(trainingSet, attributes, gcnew Dictionary<String^, String^>());
}

void DecisionTree::printDecisions() {
	String^ decisionString = "";
	for each (KeyValuePair<String^, String^>^ decision in decisions) {
		decisionString += decision->Key + ": " + decision->Value;
	}
}