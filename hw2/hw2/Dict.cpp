#include "StdAfx.h"
#include "Dict.h"

using namespace System::IO;

Dict::Dict(String^ dictPath) {
	dict = gcnew HashSet<String^>();
	costs = gcnew Dictionary<String^, Dictionary<String^, int>^>();

	// Read in the dictionary
	StreamReader^ dictFile = gcnew StreamReader(dictPath);

	while (String^ item = dictFile->ReadLine()) {
		dict->Add(item);
	}
}

String^ Dict::getCorrectWord(String^ word) {
	if (dict->Contains(word)) {
		return word;
	}

	// Find the word with the smallest Levenshtein Distance
	return "";
}

void Dict::addCost(String^ word1, String^ word2, int cost) {
	if (costs->ContainsKey(word1)) {
		Dictionary<String^, int>^ matches = costs[word1];
		matches->Add(word2, cost);
	} else {
		Dictionary<String^, int>^ matches = gcnew Dictionary<String^, int>();
		matches->Add(word2, cost);
		costs->Add(word1, matches);
	}

	if (costs->ContainsKey(word2)) {
		Dictionary<String^, int>^ matches = costs[word2];
		matches->Add(word1, cost);
	} else {
		Dictionary<String^, int>^ matches = gcnew Dictionary<String^, int>();
		matches->Add(word1, cost);
		costs->Add(word2, matches);
	}
}

int Dict::getCost(String^ word1, String^ word2) {
	if (costs->ContainsKey(word1)) {
		Dictionary<String^, int>^ matches = costs[word1];
		if (costs->ContainsKey(word2)) {
			return matches[word2];
		}
	}

	return -1;
}