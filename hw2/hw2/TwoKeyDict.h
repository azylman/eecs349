#pragma once

using namespace System;
using namespace System::Collections::Generic;

// Visual C++ doesn't support multi-dimensional arrays
generic<typename K, typename V>
public ref class TwoKeyDict
{
private:
	Dictionary<K, Dictionary<K, V>^>^ underlyingDict;
public:
	TwoKeyDict() {
		underlyingDict = gcnew Dictionary<K, Dictionary<K, V>^>();
	}
	V Get(K key1, K key2) {
		if (underlyingDict->ContainsKey(key1)) {
			if (underlyingDict[key1]->ContainsKey(key2)) {
				return underlyingDict[key1][key2];
			}
		}

		//Console::WriteLine("Getting value of " + key1 + " & " + key2 + ": " + V());
		return V();
	}
	void Put(K key1, K key2, V value) {
		if (underlyingDict->ContainsKey(key1)) {
			Dictionary<K, V>^ secondLayerDict = underlyingDict[key1];
			if (secondLayerDict->ContainsKey(key2)) {
				secondLayerDict[key2] = value;
			} else {
				secondLayerDict->Add(key2, value);
			}
		} else {
			Dictionary<K, V>^ secondLayerDict = gcnew Dictionary<K, V>();
			secondLayerDict->Add(key2, value);
			underlyingDict->Add(key1, secondLayerDict);
		}
	}
};

