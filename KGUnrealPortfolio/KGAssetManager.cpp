// Fill out your copyright notice in the Description page of Project Settings.


#include "KGAssetManager.h"
#include "KGUtil.h"

UKGAssetManager::UKGAssetManager()
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
}

UKGAssetManager& UKGAssetManager::Get()
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

	UKGAssetManager* Singleton = Cast<UKGAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(KGLog, Fatal, TEXT("Cannot use AssetManager if no AssetManagerClassName is defined!"));
		return *NewObject<UKGAssetManager>(); // never calls this
	}
}

void UKGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
}

void UKGAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

UDataTable* UKGAssetManager::FindDataTable(const FName& TableName) const
{
	FPrimaryAssetType AssetType = FPrimaryAssetType(TEXT("DataTable"));

	TArray<FPrimaryAssetId> AssetIds;
	GetPrimaryAssetIdList(AssetType, AssetIds);
	TArray<FPrimaryAssetId>::TIterator iter = AssetIds.CreateIterator();
	for (;iter; ++iter)
	{
		FString AssetName = iter->PrimaryAssetName.ToString();
		if (0 == AssetName.Compare(TableName.ToString(), ESearchCase::CaseSensitive))
		{
			FAssetData AssetData;

			GetPrimaryAssetData(*iter, AssetData);
			UDataTable* Table = Cast<UDataTable>(AssetData.GetAsset());

			if (Table)
			{
				return Table;
			}
			else
			{
				Table = Cast<UDataTable>(GetPrimaryAssetObject(*iter));
				return Table;
			}
		}
	}

	return nullptr;
}
