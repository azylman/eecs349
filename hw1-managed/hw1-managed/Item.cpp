#include "StdAfx.h"
#include "Item.h"

Item::Item(Dictionary<String^, String^>^ attributes) : attributes(attributes) {
}

String^ Item::GetAttribute(String^ attribute) {
	return attributes[attribute];
}