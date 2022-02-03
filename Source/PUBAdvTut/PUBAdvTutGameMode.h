// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "ItemBase.h"
#include "PUBAdvTutGameMode.generated.h"

class AItemsGroup;








UCLASS(minimalapi)
class APUBAdvTutGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APUBAdvTutGameMode();

	void GenerateItems();

	int32 RandomItemNumber();

	E_ItemType RandomItemType();

	FSTR_ItemTypesAndID* RandomItemID(E_ItemType ItemTypex1);


private:
	FString ItemsGLTablePath;

	UDataTable* ItemsGLTableObject;

	FString NumbersPBTablePath;

	UDataTable* NumbersPBTableObject;

	FString ItemTypePBTablePath;

	UDataTable* ItemTypePBTableObject;

	FString WeaponDTablePath;

	UDataTable* WeaponDTableObject;

	FString WeaponAccDTablePath;

	UDataTable* WeaponAccDTableObject;

	FString AmmoDTablePath;

	UDataTable* AmmoDTableObject;

	FString HealthDTablePath;

	UDataTable* HealthDTableObject;

	FString BoostDTablePath;

	UDataTable* BoostDTableObject;

	FString EquipmentDTablePath;

	UDataTable* EquipmentDTableObject;




protected:

	virtual void BeginPlay() override;

};



