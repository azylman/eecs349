#include "StdAfx.h"
#include "DecisionTree.h"

void DecisionTree::build(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes) {
	if (trainingSet->Count() == 0) {
		label = trainingSet->mostCommonClassification();
	} else if (trainingSet->allPositive()) {
		label = "true";
	} else if (trainingSet->allNegative()) {
		label = "false";
	} else if (attributes->Count == 0) {
		label = trainingSet->mostCommonClassification();
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

bool DecisionTree::classify(Item^ item) {
	if (children->Count > 0) {
		String^ attribute = item->GetAttribute(decisionAttribute);
		return children[attribute]->classify(item);
	} else {
		return Item::valueIsPositive(label) == item->isPositive();
	}
}

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

double DecisionTree::classifyTestSet() {
	if (testingSet->Count() > 0) {
		return classify(testingSet);
	} else {
		return -1;
	}
}

double DecisionTree::priorClassify(Items^ dataset) {
	int correct = 0;
	int incorrect = 0;

	Random^ rnd = gcnew Random();
	for each (Item^ item in dataset->getItems()) {
		double num = rnd->NextDouble();
		bool classifiedAsPositive = priorProbability > num;
		if (classifiedAsPositive == item->isPositive()) {
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

double DecisionTree::priorClassifyTestSet() {
	return priorClassify(testingSet);
}

void DecisionTree::setVerbosePrinting(bool verbose) {
	this->verbose = verbose;
}