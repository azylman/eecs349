#include "StdAfx.h"
#include "Dict.h"

using namespace System::IO;

Dict::Dict(String^ dictPath) {
	dict = gcnew HashSet<String^>();
	costs = gcnew UnorderedTwoKeyDict<String^, int>();
	correctWords = gcnew Dictionary<String^, String^>();

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

	if (correctWords->ContainsKey(word)) {
		return correctWords[word];
	}

	// Find the word with the smallest Levenshtein Distance
	int shortestDistance = int::MaxValue;
	String^ bestWord = "";
	for each (String^ alternateWord in dict) {
		int cost = getCost(word, alternateWord);

		if (cost == 0) {
			cost = calculateLevenshteinDistance(word, alternateWord);
			addCost(word, alternateWord, cost);
		}

		if (cost < shortestDistance) {
			bestWord = alternateWord;
			shortestDistance = cost;
		}
	}

	return bestWord;
}

// These are both currently wrong. We're using a TwoKeyDict, which assumes that the order of the keys matters.
// We need to subclass it and create one that doesn't care about the order of the keys.
void Dict::addCost(String^ word1, String^ word2, int cost) {
	costs->Put(word1, word2, cost);
}

int Dict::getCost(String^ word1, String^ word2) {
	return costs->Get(word1, word2);
}

int Dict::calculateLevenshteinDistance(String^ s, String^ t) {

	int m = s->Length;
	int n = t->Length;

	TwoKeyDict<Int32, Int32>^ d = gcnew TwoKeyDict<Int32, Int32>();
	int deletionCost = 1;
	int insertionCost = 1;
	int substitutionCost = 1;

	for (int i = 0; i < m; ++i) {
		d->Put(i, 0, i); // the distance of any first string to an empty second string
	}
	for (int i = 0; i < n; ++i) {
		d->Put(0, i, i); // the distance of any second string to an empty first string
	}

	for (int j = 1; j < n; ++j) {
		for (int i = 1; i < m; ++i) {
			if (s[i] == t[j]) {
				int cost = d->Get(i - 1, j - 1);
				d->Put(i, j, cost); // no operation cost, because they match
			} else {
				int cost = Math::Min(
								Math::Min(
									d->Get(i - 1, j) + deletionCost,
									d->Get(i, j - 1) + insertionCost),
								d->Get(i - 1, j - 1) + substitutionCost);
				d->Put(i, j, cost);
			}
		}
	}

	return d->Get(m-1, n-1);
}