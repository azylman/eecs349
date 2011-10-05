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
	void print(bool verbose, int depth);
	void build(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes);
	void initValues();
	bool classify(Item^ item);
	double priorProbability;
	bool verbose;

public:
	DecisionTree(Items^ trainingSet, Items^ testingSet, Dictionary<String^, List<String^>^>^ attributes);
	DecisionTree(Items^ trainingSet, Dictionary<String^, List<String^>^>^ attributes);
	void print();
	double classify(Items^ dataset);
	double classifyTestSet();
	double priorClassify(Items^ dataset);
	double priorClassifyTestSet();
	void setVerbosePrinting(bool verbose);
};

