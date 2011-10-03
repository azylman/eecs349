#include "StdAfx.h"
#include "Items.h"


Items::Items(void) {
	items = gcnew List<Item^>();
}

void Items::Add(Item^ item) {
	items->Add(item);
}