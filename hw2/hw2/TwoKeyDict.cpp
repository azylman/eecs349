#include "StdAfx.h"
#include "TwoKeyDict.h"

template<class K, class V>
TwoKeyDict<K, V>::TwoKeyDict(void) {
}

template<class K, class V>
V TwoKeyDict<K, V>::Get(K key1, K key2) {
	K firstKey = getFirstKey(key1, key2);
	K secondKey = getSecondKey(key1, key2);

	if (underlyingDict->ContainsKey(firstKey)) {
		if (underlyingDict[firstKey]->ContainsKey(secondKey)) {
			return underlyingDict[firstKey][secondKey];
		}
	}

	return 0;
}

template<class K, class V>
void TwoKeyDict<K, V>::Put(K key1, K key2, V value) {
	K firstKey = getFirstKey(key1, key2);
	K secondKey = getSecondKey(key1, key2);

	if (underlyingDict->ContainsKey(firstKey)) {
		Dictionary<K, V>^>^ secondLayerDict = underlyingDict[firstKey];
		if (secondLayerDict->ContainsKey(secondKey)) {
			secondLayerDict[secondKey] = value;
		} else {
			secondLayerDict->Add(secondKey, value);
		}
	} else {
		Dictionary<K, V>^>^ secondLayerDict = gcnew Dictionary<K, V>^>();
		secondLayerDict->Add(secondKey, value);
		underlyingDict->Add(firstKey, secondLayerDict);
	}
}

template<class K, class V>
K TwoKeyDict<K, V>::getFirstKey(K key1, K key2) {
	return key1 <= key2 ? key1 : key2;
}

template<class K, class V>
K TwoKeyDict<K, V>::getSecondKey(K key1, K key2) {
	return key1 >= key2 ? key1 : key2;
}