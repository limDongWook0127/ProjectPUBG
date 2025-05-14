// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DataTableManager.h"


UDataTableManager::UDataTableManager()
{
}

void UDataTableManager::InitializeManager()
{
}

UDataTable* UDataTableManager::GetDataTable(EDataTableType TableName) 
{
	if (UDataTable** TablePtr = DataTables.Find(TableName))
	{
		UDataTable* Table = *TablePtr; // 안전하게 값 가져오기
		return Table;
	}

	UE_LOG(LogTemp, Warning, TEXT("Table not found!"));
	return nullptr;	
}

