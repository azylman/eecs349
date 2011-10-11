#pragma once

using namespace System::Collections::Generic;

// Visual C++ doesn't support multi-dimensional arrays
template<class K, class V>
ref class TwoKeyDict
{
private:
	K getFirstKey(K key1, K key2);
	K getSecondKey(K key1, K key2);
	Dictionary<K, Dictionary<K, V>^>^ underlyingDict;
public:
	TwoKeyDict<K, V>(void);
	V Get(K key1, K key2);
	void Put(K key1, K key2, V value);
};

