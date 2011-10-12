#pragma once

#include "TwoKeyDict.h"

using namespace System;

// A TwoKeyDict that doesn't care what order the keys are
generic<typename K, typename V>
where K : IComparable
public ref class UnorderedTwoKeyDict
{
private:
	TwoKeyDict<K, V>^ underlyingDict;

	K getFirstKey(K key1, K key2) {
		return key1->CompareTo(key2) ? key1 : key2;
	}
	K getSecondKey(K key1, K key2) {
		return key1->CompareTo(key2) ? key2 : key1;
	}
public:
	UnorderedTwoKeyDict() {
		underlyingDict = gcnew TwoKeyDict<K, V>();
	}
	V Get(K key1, K key2) {
		K firstKey = getFirstKey(key1, key2);
		K secondKey = getSecondKey(key1, key2);
		return underlyingDict->Get(firstKey, secondKey);
	}
	void Put(K key1, K key2, V value) {
		K firstKey = getFirstKey(key1, key2);
		K secondKey = getSecondKey(key1, key2);
		underlyingDict->Put(firstKey, secondKey, value);
	}
};

