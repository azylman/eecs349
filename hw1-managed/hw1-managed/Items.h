#pragma once

#include "Item.h"

ref class Items
{
private:
	List<Item^>^ items;
	List<String^>^ attributes;
	double calculateEntropy(int positive, int negative);
	double calculateEntropyGainFromAttribute(Dictionary<String^, String^>^ usedClassifications, String^ attribute);
	void addOrIncrementKey(Dictionary^<String^, int> dict, String^ key);
public:
	Items(List<String^>^ attributes);
	void Add(Item^ item);
	String^ getBestClassifer(Dictionary<String^, String^>^ usedClassifications);
};

