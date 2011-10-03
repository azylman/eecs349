#pragma once

#include "Item.h"

ref class Items
{
private:
	List<Item^>^ items;
	double calculateEntropy(int positive, int negative);
public:
	Items(void);
	void Add(Item^ item);
	String^ getBestClassifer(Dictionary<String^, String^> assumedClassifications);
};

