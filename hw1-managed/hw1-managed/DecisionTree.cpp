#include "StdAfx.h"
#include "DecisionTree.h"


DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, String^>^ decisions) : trainingSet(trainingSet), decisions(decisions) {
	if (trainingSet->allPositive()) {
		label = "+";
	} else if (trainingSet->allNegative()) {
		label = "-";
	} else if (trainingSet->attributesEmpty()) {
		label = trainingSet->mostCommonClassification();
	} else {
	}
}

DecisionTree::DecisionTree(Items^ trainingSet) {
	DecisionTree(trainingSet, gcnew Dictionary<String^, String^>());
}
