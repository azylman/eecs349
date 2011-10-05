#include "StdAfx.h"
#include "Items.h"

using namespace System;
using namespace System::Collections::Generic;

Items::Items() {
	items = gcnew List<Item^>();
}

double Items::calculateEntropy(int positive, int negative) {
	double good = (double) positive / (double) (positive + negative);
	double bad = (double) negative / (double) (positive + negative);
	double goodLog = good == 0 ? 0 : -good * Math::Log(good, 2);
	double badLog = bad == 0 ? 0 : -bad * Math::Log(bad, 2);
	return goodLog + badLog;
}

double Items::calculateEntropyGainFromAttribute(String^ attribute) {

	int originalGood = 0;
	int originalBad = 0;
	Dictionary<String^, int>^ goodCountsByAttribute = gcnew Dictionary<String^, int>();
	Dictionary<String^, int>^ badCountsByAttribute = gcnew Dictionary<String^, int>();
	Dictionary<String^, int>^ totalItemsByAttribute = gcnew Dictionary<String^, int>();

	for each (Item^ item in items) {
		if (valueIsPositive(item->GetAttribute("CLASS"))) {
			originalGood++;

			addOrIncrementKey(goodCountsByAttribute, item->GetAttribute(attribute));
			addOrIncrementKey(totalItemsByAttribute, item->GetAttribute(attribute));
		} else {
			originalBad++;

			addOrIncrementKey(badCountsByAttribute, item->GetAttribute(attribute));
			addOrIncrementKey(totalItemsByAttribute, item->GetAttribute(attribute));
		}
	}

	int totalItems = originalGood + originalBad;

	double totalEntropy = calculateEntropy(originalGood, originalBad);
	for each (KeyValuePair<String^, int>^ attributeCount in totalItemsByAttribute) {
		String^ key = attributeCount->Key;
		int value = attributeCount->Value;
		int goodCounts = goodCountsByAttribute->ContainsKey(key) ? goodCountsByAttribute[key] : 0;
		int badCounts = badCountsByAttribute->ContainsKey(key) ? badCountsByAttribute[key] : 0;
		double modifier = (double) value / (double) totalItems;
		double entropy =  modifier * calculateEntropy(goodCounts, badCounts);
		totalEntropy -= entropy;
	}
	return totalEntropy;
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

void Items::Add(List<Item^>^ items) {
	this->items->AddRange(items);
}

String^ Items::getBestClassifer(Dictionary<String^, List<String^>^>^ attributes) {
	List<String^>^ attributesToCheck = gcnew List<String^>(attributes->Keys);
	if (attributesToCheck->Count == 1) {
		return attributesToCheck[0];
	}

	double maxGain = 0;
	String^ bestAttribute;

	for each(String^ attribute in attributesToCheck) {
		double gain = calculateEntropyGainFromAttribute(attribute);
		if (gain > maxGain) {
			maxGain = gain;
			bestAttribute = attribute;
		}
	}

	return bestAttribute;
}

Items^ Items::getTrainingSet(int trainingSetSize) {
	Items^ trainingSet = gcnew Items();
	if (trainingSetSize > items->Count) {
		// We should really probably throw an error here, but it should never come up.
		trainingSet->Add(items);
		return trainingSet;
	}
	HashSet<int>^ indicesToTake = getNRandomNumbersFromAToB(trainingSetSize, 0, items->Count);
	for each(int i in indicesToTake) {
		trainingSet->Add(items[i]);
	}
	trainingSet->setAttributes(attributes);
	return trainingSet;
}

HashSet<int>^ Items::getNRandomNumbersFromAToB(int n, int a, int b) {
	HashSet<int>^ result = gcnew HashSet<int>();
	Random^ rnd = gcnew Random();
	while(result->Count < n) {
		result->Add(rnd->Next(a, b));
	}
	return result;
}

Items^ Items::getTestingSet(Items^ trainingSet) {
	Items^ testingSet = gcnew Items();
	testingSet->Add(setSubtract(items, trainingSet->items));
	trainingSet->setAttributes(attributes);
	return testingSet;
}

generic<typename T>
List<T>^ Items::setSubtract(List<T>^ one, List<T>^ two) {
	List<T>^ result = gcnew List<T>(one);
	for each (T item in two) {
		result->Remove(item);
	}
	return result;
}

Items^ Items::filterListByDecisions(Dictionary<String^, String^>^ decisions) {
	Items^ filteredItems = gcnew Items();
	if (decisions->Count > 0) {
		for each(Item^ item in items) {
			bool add = true;
			for each(KeyValuePair<String^, String^>^ classification in decisions) {
				if (!item->GetAttribute(classification->Key)->Equals(classification->Value)) {
					add = false;
				}
			}
			if (add) {
				filteredItems->Add(item);
			}
		}
	} else {
		return this;
	}
	filteredItems->setAttributes(attributes);
	return filteredItems;
}

bool Items::allPositive() {
	for each (Item^ item in items) {
		if (!valueIsPositive(item->GetAttribute("CLASS"))) {
			return false;
		}
	}
	return true;
}

bool Items::allNegative() {
	for each (Item^ item in items) {
		if (valueIsPositive(item->GetAttribute("CLASS"))) {
			return false;
		}
	}
	return true;
}

String^ Items::mostCommonClassification() {
	int zeroes = 0;
	int ones = 0;
	for each (Item^ item in items) {
		if (valueIsPositive(item->GetAttribute("CLASS"))) {
			ones++;
		} else {
			zeroes++;
		}
	}

	return ones > zeroes ? "true" : "false";
}

void Items::setAttributes(Dictionary<String^, List<String^>^>^ attributes) {
	this->attributes = attributes;
}

int Items::Count() {
	return items->Count;
}

String^ Items::ToString() {
	String^ result = "{";
	for each (Item^ item in items) {
		result += item->ToString() + "\n";
	}
	return result->Trim(',') + "}";
}

bool Items::valueIsPositive(String^ value) {
	return value->Equals("1") || value->ToLower()->Equals("true");
}