#include "StdAfx.h"
#include "Items.h"

using namespace System;
using namespace System::Collections::Generic;

Items::Items() {
	items = gcnew List<Item^>();
}

// Calculate entropy from a given amount of positive and negative classifications
double Items::calculateEntropy(int positive, int negative) {
	double good = (double) positive / (double) (positive + negative);
	double bad = (double) negative / (double) (positive + negative);

	// Don't try to calculate the log if the counts are 0, just return 0
	double goodLog = good == 0 ? 0 : -good * Math::Log(good, 2);
	double badLog = bad == 0 ? 0 : -bad * Math::Log(bad, 2);

	return goodLog + badLog;
}

// Calculates the entropy gain from a specific attribute
double Items::calculateEntropyGainFromAttribute(String^ attribute) {

	int originalGood = 0;
	int originalBad = 0;

	// Keep track of the amount of each attribute value we see for calculating the entropy multiplier
	Dictionary<String^, int>^ goodCountsByAttributeValue = gcnew Dictionary<String^, int>();
	Dictionary<String^, int>^ badCountsByAttributeValue = gcnew Dictionary<String^, int>();
	Dictionary<String^, int>^ totalItemsByAttributeValue = gcnew Dictionary<String^, int>();

	for each (Item^ item in items) {
		if (item->isPositive()) {
			originalGood++;

			addOrIncrementKey(goodCountsByAttributeValue, item->GetAttribute(attribute));
			addOrIncrementKey(totalItemsByAttributeValue, item->GetAttribute(attribute));
		} else {
			originalBad++;

			addOrIncrementKey(badCountsByAttributeValue, item->GetAttribute(attribute));
			addOrIncrementKey(totalItemsByAttributeValue, item->GetAttribute(attribute));
		}
	}

	int totalItems = originalGood + originalBad;

	double totalEntropy = calculateEntropy(originalGood, originalBad);

	for each (KeyValuePair<String^, int>^ attributeCount in totalItemsByAttributeValue) {
		// The attribute value we're looking at
		String^ key = attributeCount->Key;
		// How many of that attribute value we found
		int value = attributeCount->Value;
		// How many positive items for that attribute value
		int goodCounts = goodCountsByAttributeValue->ContainsKey(key) ? goodCountsByAttributeValue[key] : 0;
		// How many negative items for that attribute value
		int badCounts = badCountsByAttributeValue->ContainsKey(key) ? badCountsByAttributeValue[key] : 0;
		// The multiplier for the entropy of this attribute value
		double modifier = (double) value / (double) totalItems;
		double entropy =  modifier * calculateEntropy(goodCounts, badCounts);
		totalEntropy -= entropy;
	}
	return totalEntropy;
}

// If a key already exists in the dictionary, increment it, otherwise, add it to the table
void Items::addOrIncrementKey(Dictionary<String^, int>^ dict, String^ key) {
	if (dict->ContainsKey(key)) {
		dict[key]++;
	} else {
		dict->Add(key, 1);
	}
}

// Add an item to the underlying list structure
void Items::Add(Item^ item) {
	items->Add(item);
}

// Add a list of items to the underlying list structure
void Items::Add(List<Item^>^ items) {
	this->items->AddRange(items);
}

// Return the attribute that currently best classifies the data
String^ Items::getBestClassifer(Dictionary<String^, List<String^>^>^ attributes) {

	// The list of all attributes that we haven't classified by in this branch
	List<String^>^ attributesToCheck = gcnew List<String^>(attributes->Keys);

	// If there's only one attribute left, it best classifies the data
	if (attributesToCheck->Count == 1) {
		return attributesToCheck[0];
	}

	double maxGain = 0;
	String^ bestAttribute;

	// Loop over all the attributes we have and return the one that has the highest information gain
	for each(String^ attribute in attributesToCheck) {
		double gain = calculateEntropyGainFromAttribute(attribute);
		// If the gain we just calculated is greater than our previously recorded maximum, update our maximum
		if (gain > maxGain) {
			maxGain = gain;
			bestAttribute = attribute;
		}
	}

	return bestAttribute;
}

Items^ Items::getTrainingSet(int trainingSetSize) {

	Items^ trainingSet = gcnew Items();
	
	// Get N random indices for a training set (where N is the training set size
	HashSet<int>^ indicesToTake = getNRandomNumbersFromAToB(trainingSetSize, 0, items->Count);

	for each(int i in indicesToTake) {
		trainingSet->Add(items[i]);
	}

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
	// Create  a testing set by returning all the items that are NOT in the training set
	testingSet->Add(setSubtract(items, trainingSet->items));
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

// Return a new list with all the elements that don't satisfy our new decision removed
Items^ Items::filterListByDecisions(Dictionary<String^, String^>^ decisions) {
	Items^ filteredItems = gcnew Items();

	// If a decision was made
	if (decisions->Count > 0) {
		for each(Item^ item in items) {
			bool add = true;
			// If any of the attribute values are different than the ones in our decision, don't add them
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

	return filteredItems;
}

// Check if all the items are positive
bool Items::allPositive() {
	for each (Item^ item in items) {
		if (!item->isPositive()) {
			return false;
		}
	}
	return true;
}

// Check if all the items are negative
bool Items::allNegative() {
	for each (Item^ item in items) {
		if (item->isPositive()) {
			return false;
		}
	}
	return true;
}

// Return the most common classification of all the items
String^ Items::mostCommonClassification() {
	int zeroes = 0;
	int ones = 0;
	for each (Item^ item in items) {
		if (item->isPositive()) {
			ones++;
		} else {
			zeroes++;
		}
	}

	return ones > zeroes ? "true" : "false";
}

int Items::Count() {
	return items->Count;
}

String^ Items::ToString() {
	String^ result = "{";
	for each (Item^ item in items) {
		result += item->ToString() + "\n";
	}
	return result->Trim('\n') + "}";
}

List<Item^>^ Items::getItems() {
	return items;
}