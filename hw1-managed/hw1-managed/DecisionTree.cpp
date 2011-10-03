#include "StdAfx.h"
#include "DecisionTree.h"


DecisionTree::DecisionTree(Items^ trainingSet, Dictionary<String^, String^>^ decisions) : trainingSet(trainingSet), decisions(decisions) {

}

DecisionTree::DecisionTree(Items^ trainingSet) {
	DecisionTree(trainingSet, gcnew Dictionary<String^, String^>());
}
