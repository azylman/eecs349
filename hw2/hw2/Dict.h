#pragma once

using namespace System;
using namespace System::Collections::Generic;

public ref class Dict
{
private:
	HashSet<String^>^ dict;
	Dictionary<String^, Dictionary<String^, int>^>^ costs;
	void addCost(String^ word1, String^ word2, int cost);
	int getCost(String^ word1, String^ word2);
public:
	Dict(String^ dictPath);
	String^ getCorrectWord(String^ word);
};

