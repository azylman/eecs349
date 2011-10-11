#include "StdAfx.h"
#include "Dict.h"

using namespace System::IO;

Dict::Dict(String^ dictPath) {
	dict = gcnew HashSet<String^>();

	// Read in the dictionary
	StreamReader^ dictFile = gcnew StreamReader(dictPath);

	while (String^ item = dictFile->ReadLine()) {
		dict->Add(item);
	}
}
