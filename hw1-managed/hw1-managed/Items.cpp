#include "StdAfx.h"
#include "Items.h"

using namespace System;
using namespace System::Collections::Generic;

Items::Items() {
	items = gcnew List<Item^>();
}

double Items::calculateEntropy(int positive, int negative) {
	double good = positive / (positive + negative);
	double bad = negative / (positive + negative);
	return -good * Math::Log(good, 2) - bad * Math::Log(bad, 2);
}

double Items::calculateEntropyGainFromAttribute(Dictionary<String^, String^>^ usedClassifications, String^ attribute) {
	List<Item^>^ filteredItems = filterListByDecisions(usedClassifications);

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
		entropy -= value / totalItems * calculateEntropy(
			goodCountsByAttribute->ContainsKey(key) ? goodCountsByAttribute[key] : 0,
			badCountsByAttribute->ContainsKey(key) ? badCountsByAttribute[key] : 0);
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

void Items::Add(List<Item^>^ items) {
	this->items->AddRange(items);
}

String^ Items::getBestClassifer(Dictionary<String^, String^>^ usedClassifications) {
	List<String^>^ attributesToCheck = setSubtract(gcnew List<String^>(attributes->Keys), gcnew List<String^>(usedClassifications->Keys));

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

List<Item^>^ Items::filterListByDecisions(Dictionary<String^, String^>^ decisions) {
	List<Item^>^ filteredItems = gcnew List<Item^>();
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
		filteredItems = items;
	}

	return filteredItems;
}

bool Items::allPositive() {
	for each (Item^ item in items) {
		if (item->GetAttribute("CLASS")->Equals("0")) {
			return false;
		}
	}
	return true;
}

bool Items::allNegative() {
	for each (Item^ item in items) {
		if (item->GetAttribute("CLASS")->Equals("1")) {
			return false;
		}
	}
	return true;
}

bool Items::attributesEmpty() {
	return attributes->Count == 0;
}

String^ Items::mostCommonClassification() {
	int zeroes = 0;
	int ones = 0;
	for each (Item^ item in items) {
		if (item->GetAttribute("CLASS")->Equals("0")) {
			zeroes++;
		} else {
			ones++;
		}
	}

	return ones > zeroes ? "1" : "0";
}

void Items::setAttributes(Dictionary<String^, List<String^>^>^ attributes) {
	this->attributes = attributes;
}