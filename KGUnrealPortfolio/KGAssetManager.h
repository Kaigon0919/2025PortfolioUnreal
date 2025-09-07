// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include <KGCharacterType.h>
#include "KGAssetManager.generated.h"

/**
 *
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UKGAssetManager();
public:
	static UKGAssetManager& Get();

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;
	/** Finishes initial loading, gets called from end of Engine::Init() */
	virtual void FinishInitialLoading()override;

	UDataTable* FindDataTable(const FName& TableName) const;

	template<class T>
	T* FindDataTableRow(const FName& TableName, const FName& RowName) const
	{
		UDataTable* DataTable = FindDataTable(TableName);
		if (DataTable)
		{
			return DataTable->FindRow<T>(RowName, TEXT(""));
		}
		return nullptr;
	}

	template<class T>
	T* FindDataTableRow(const FName& TableName, const EKGCharacterType RowName) const
	{
		UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EKGCharacterType"), true);
		if (!EnumPtr) return nullptr;

		UDataTable* DataTable = FindDataTable(TableName);
		FName rowName = *(EnumPtr->GetNameStringByValue(static_cast<int64>(RowName)));
		if (DataTable)
		{
			return DataTable->FindRow<T>(rowName, TEXT(""));
		}
		return nullptr;
	}
};
