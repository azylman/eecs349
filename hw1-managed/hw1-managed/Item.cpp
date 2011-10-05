#include "StdAfx.h"
#include "Item.h"

Item::Item(Dictionary<String^, String^>^ attributes) : attributes(attributes) {
}

String^ Item::GetAttribute(String^ attribute) {
	return attributes[attribute];
}

String^ Item::ToString() {
	String^ string = "[";
	for each (KeyValuePair<String^, String^>^ attribute in attributes) {
		string += attribute->Value + " ";
	}
	return string->Trim() + "]";
}

// Returns whether or not the item is positive
bool Item::isPositive() {
	return valueIsPositive(attributes["CLASS"]);
}

// The conditions for specifying positive vs negative - currently we support "1" (for Cars.txt) and "true" (for MajorityRule and IvyLeague)
bool Item::valueIsPositive(String^ value) {
	return value->Equals("1") || value->ToLower()->Equals("true");
}