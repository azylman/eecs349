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