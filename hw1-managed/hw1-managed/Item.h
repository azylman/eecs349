#pragma once

using namespace System;
using namespace System::Collections::Generic;

public ref class Item
{
private:
	Dictionary<String^, String^>^ attributes;
public:
	Item(Dictionary<String^, String^>^ attributes);
	String^ GetAttribute(String^ attribute);
	virtual String^ ToString() override;
	bool isPositive();
	static bool valueIsPositive(String^ value);
};

