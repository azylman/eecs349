#pragma once

#include "Item.h"

ref class Items
{
private:
	List<Item^>^ items;
public:
	Items(void);
	void Add(Item^ item);
	String^ getBestClassifer(Dictionary<String^, String^> assumedClassifications);
};

