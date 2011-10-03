#pragma once

#include "Item.h"

ref class Items
{
private:
	List<Item^>^ items;
	List<String^>^ attributes;
	double calculateEntropy(int positive, int negative);
public:
	Items(List<String^>^ attributes);
	void Add(Item^ item);
	String^ getBestClassifer(Dictionary<String^, String^> usedClassifications);
};

