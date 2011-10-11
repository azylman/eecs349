#pragma once

using namespace System;
using namespace System::Collections::Generic;

public ref class Dict
{
	HashSet<String^>^ dict;
public:
	Dict(String^ dictPath);
	String^ getCorrectWord(String^ word);
};

