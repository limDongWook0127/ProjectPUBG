// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/DDInventorySlot.h"

void UDDInventorySlot::UpdateDDInventorySlot()
{
	
}

int32 UDDInventorySlot::GetHaveComponent()
{
	if (NearComponent != nullptr)
	{
		return 1; // NearComponent를 가지고 있다는 것 
	}
	else if (InventoryComponent != nullptr)
	{
		return 2; 
	}
	//  todo : EquippedComponent 추가

	return -1;
}
