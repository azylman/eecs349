#pragma once

// A TwoKeyDict that doesn't care what order the keys are
generic<typename K, typename V>
public ref class UnorderedTwoKeyDict
{
private:
	TwoKeyDict<K, V>^ underlyingDict;

	K getFirstKey(K key1, K key2) {
		return key1 >= key2 ? key1 : key2;
	}
	K getSecondKey(K key1, K key2) {
		return key1 >= key2 ? key2 : key1;
	}
public:
	UnorderedTwoKeyDict(void);
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

