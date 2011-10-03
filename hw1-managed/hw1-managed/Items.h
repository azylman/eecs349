#pragma once

#include "Item.h"

ref class Items
{
private:
	List<Item^>^ items;
	List<String^>^ attributes;
	double calculateEntropy(int positive, int negative);
	double calculateEntropyGainFromAttribute(Dictionary<String^, String^>^ usedClassifications, String^ attribute);
	void addOrIncrementKey(Dictionary<String^, int>^ dict, String^ key);
	HashSet<int>^ getNRandomNumbersFromAToB(int n, int a, int b);
	generic<typename T>
	List<T>^ setSubtract(List<T>^ one, List<T>^ two);
	List<Item^>^ filterListByDecisions(Dictionary<String^, String^>^ decisions);
public:
	Items(List<String^>^ attributes);
	void Add(Item^ item);
	void Add(List<Item^>^ items);
	String^ getBestClassifer(Dictionary<String^, String^>^ usedClassifications);
	Items^ getTrainingSet(int trainingSetSize);
	Items^ getTestingSet(Items^ trainingSet);
};

