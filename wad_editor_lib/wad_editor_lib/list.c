//
//  list.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

WE_LIST* listCreate(WE_LIST_DELEGATE* delegate)
{
	WE_LIST* list = malloc(sizeof(WE_LIST));
	list->delegate = *delegate;
	debug_memset(&list->subscriber, 0, sizeof(WE_LIST_SUBSCRIBER));
	list->hasSubscriber = 0;
	
	list->itemAllocator = dataAllocatorCreate(sizeof(WE_LIST_ITEM), 512);
	listItemInitialize(&list->rootItem, list, NULL, 0, NULL, "Root");
	
	list->selectedItem = NULL;
	
	return list;
}

void listRelease(WE_LIST* list)
{
	assert(list);
	
	listItemDeinitialize(&list->rootItem);
	dataAllocatorRelease(list->itemAllocator);
	
	debug_memset(list, 0, sizeof(WE_LIST));
	free(list);
}


void listSubscribe(WE_LIST* list, WE_LIST_SUBSCRIBER* subscriber)
{
	assert(list);
	assert(subscriber);
	
	list->subscriber = *subscriber;
	list->hasSubscriber = 1;
}

void listSelectItem(WE_LIST* list, WE_LIST_ITEM* item)
{
	assert(list);
	
	if (list->selectedItem)
	{
		WE_LIST_ITEM* lastSelectedItem = list->selectedItem;
		list->selectedItem = NULL;
		if (list->hasSubscriber)
		{
			list->subscriber.itemDeselectedFunc(list, lastSelectedItem, list->subscriber.userInfo);
		}
	}
	
	if (item)
	{
		list->selectedItem = item;
		if (list->hasSubscriber)
		{
			list->subscriber.itemSelectedFunc(list, item, list->subscriber.userInfo);
		}
	}
}


WE_LIST_ITEM* listGetRootItem(WE_LIST* list)
{
	assert(list);
	return &list->rootItem;
}


void listClear(WE_LIST* list)
{
	assert(list);
	listItemClearChildren(&list->rootItem);
}
