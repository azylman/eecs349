#include "StdAfx.h"
#include "Items.h"

using namespace System;
using namespace System;

Items::Items(List<String^>^ attributes) : attributes(attributes) {
	items = gcnew List<Item^>();
}

double Items::calculateEntropy(int positive, int negative) {
	double good = positive / (positive + negative);
	double bad = negative / (positive + negative);
	return -good * Math::Log(good, 2) - bad * Math::Log(bad, 2);
}

void Items::Add(Item^ item) {
	items->Add(item);
}

String^ Items::getBestClassifer(Dictionary<String^, String^> usedClassifications) {
	return "";
}