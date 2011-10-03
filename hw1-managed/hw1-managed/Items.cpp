#include "StdAfx.h"
#include "Items.h"

using namespace System;
using namespace Math;

Items::Items(void) {
	items = gcnew List<Item^>();
}

double Items::calculateEntropy(int positive, int negative) {
	double good = positive / (positive + negative);
	double bad = negative / (positive + negative);
	return -good * Log(good, 2) - bad * Log(bad, 2);
}

void Items::Add(Item^ item) {
	items->Add(item);
}

String^ Items::getBestClassifer(Dictionary<String^, String^> assumedClassifications) {

}