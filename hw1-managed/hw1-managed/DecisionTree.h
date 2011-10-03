#pragma once

#include "Items.h"

using namespace System::Collections::Generic;

ref class DecisionTree
{
private:
	Items^ trainingSet;
	Dictionary<String^, String^>^ decisions;
public:
	DecisionTree(Items^ trainingSet);
	DecisionTree(Items^ trainingSet, Dictionary<String^, String^>^ decisions);
};

