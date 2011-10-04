#pragma once

#include "Item.h"

ref class Items
{
private:
	List<Item^>^ items;
	Dictionary<String^, List<String^>^>^ attributes;
	double calculateEntropy(int positive, int negative);
	double calculateEntropyGainFromAttribute(String^ attribute);
	void addOrIncrementKey(Dictionary<String^, int>^ dict, String^ key);
	HashSet<int>^ getNRandomNumbersFromAToB(int n, int a, int b);
	generic<typename T>
	List<T>^ setSubtract(List<T>^ one, List<T>^ two);
public:
	Items();
	void Add(Item^ item);
	void Add(List<Item^>^ items);
	String^ getBestClassifer(Dictionary<String^, List<String^>^>^ attributes);
	Items^ getTrainingSet(int trainingSetSize);
	Items^ getTestingSet(Items^ trainingSet);
	bool allPositive();
	bool allNegative();
	String^ mostCommonClassification();
	void setAttributes(Dictionary<String^, List<String^>^>^ attributes);
	Items^ filterListByDecisions(Dictionary<String^, String^>^ decisions);
	int Count();
};

