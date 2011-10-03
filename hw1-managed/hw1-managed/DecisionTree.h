#pragma once

#include "Items.h"

using namespace System::Collections::Generic;

ref class DecisionTree
{
private:
	Items^ trainingSet;
	Dictionary<String^, String^>^ decisions;
	String^ label;
	String^ decisionAttribute;
	Dictionary<String^, List<String^>^>^ attributes;
	List<DecisionTree^>^ children;

	void printDecisions();

public:
	DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes);
	DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions);
};

