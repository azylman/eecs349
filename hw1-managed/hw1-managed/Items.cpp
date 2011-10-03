#include "StdAfx.h"
#include "Items.h"

using namespace System;
using namespace System::Collections::Generic;

Items::Items(List<String^>^ attributes) : attributes(attributes) {
	items = gcnew List<Item^>();
}

double Items::calculateEntropy(int positive, int negative) {
	double good = positive / (positive + negative);
	double bad = negative / (positive + negative);
	return -good * Math::Log(good, 2) - bad * Math::Log(bad, 2);
}

double Items::calculateEntropyGainFromAttribute(Dictionary<String^, String^>^ usedClassifications, String^ attribute) {
	List<Item^>^ filteredItems = gcnew List<Item^>();
	if (usedClassifications->Count > 0) {
		for each(Item^ item in items) {
			bool add = true;
			for each(KeyValuePair<String^, String^>^ classification in usedClassifications) {
				if (!item->GetAttribute(classification->Key)->Equals(classification->Value)) {
					add = false;
				}
			}
			if (add) {
				filteredItems->Add(item);
			}
		}
	} else {
		filteredItems = items;
	}

	int originalGood = 0;
	int originalBad = 0;
	Dictionary<String^, int>^ goodCountsByAttribute = gcnew Dictionary<String^, int>();
	Dictionary<String^, int>^ badCountsByAttribute = gcnew Dictionary<String^, int>();
	Dictionary<String^, int>^ totalItemsByAttribute = gcnew Dictionary<String^, int>();

	for each (Item^ item in filteredItems) {
		if (item->GetAttribute("CLASS")->Equals("1")) {
			originalGood++;

			addOrIncrementKey(goodCountsByAttribute, item->GetAttribute(attribute));
			addOrIncrementKey(totalItemsByAttribute, item->GetAttribute(attribute));
		} else if (item->GetAttribute("CLASS")->Equals("0")) {
			originalBad++;

			addOrIncrementKey(badCountsByAttribute, item->GetAttribute(attribute));
			addOrIncrementKey(totalItemsByAttribute, item->GetAttribute(attribute));
		} else {
			// We're assuming that our dataset is valid - otherwise, this should throw an exception.
		}
	}

	int totalItems = originalGood + originalBad;

	double entropy = calculateEntropy(originalGood, originalBad);
	for each (KeyValuePair<String^, int>^ attributeCount in totalItemsByAttribute) {
		String^ key = attributeCount->Key;
		int value = attributeCount->Value;
		entropy -= value / totalItems * calculateEntropy(goodCountsByAttribute[key], badCountsByAttribute[key]);
	}
	return entropy;
}

void Items::addOrIncrementKey(Dictionary<String^, int>^ dict, String^ key) {
	if (dict->ContainsKey(key)) {
		dict[key]++;
	} else {
		dict->Add(key, 1);
	}
}

void Items::Add(Item^ item) {
	items->Add(item);
}

String^ Items::getBestClassifer(Dictionary<String^, String^>^ usedClassifications) {
	List<String^>^ attributesToCheck = gcnew List<String^>(attributes);

	// Set difference for lists, basically. Check all the attributes we haven't already used.
	for each(String^ attribute in usedClassifications->Keys) {
		attributesToCheck->Remove(attribute);
	}

	double maxGain = 0;
	String^ bestAttribute;

	for each(String^ attribute in attributesToCheck) {
		double gain = calculateEntropyGainFromAttribute(usedClassifications, attribute);
		if (gain > maxGain) {
			maxGain = gain;
			bestAttribute = attribute;
		}
	}

	return bestAttribute;
}