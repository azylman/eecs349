#pragma once

#include "Items.h"

using namespace System::Collections::Generic;

ref class DecisionTree
{
private:
	Items^ trainingSet;
	Items^ testingSet;
	String^ label;
	String^ decisionAttribute;
	Dictionary<String^, DecisionTree^>^ children;
	void print(int depth);
	void build(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions);

public:
	DecisionTree(Items^ trainingSet, Items^ testingSet, Dictionary<String^, List<String^>^>^ attributes);
	DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes, Dictionary<String^, String^>^ decisions);
	void print();
};

