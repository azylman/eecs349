#pragma once

#include "UnorderedTwoKeyDict.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class Dict
{
private:
	HashSet<String^>^ dict;
	UnorderedTwoKeyDict<String^, int>^ costs;
	Dictionary<String^, String^>^ correctWords;

	void addCost(String^ word1, String^ word2, int cost);
	int getCost(String^ word1, String^ word2);
	int calculateLevenshteinDistance(String^ s, String^ t);
public:
	Dict(String^ dictPath);
	String^ getCorrectWord(String^ word);
};

