

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "ItemBase.h"
#include "PUBAdvTutGameMode.generated.h"

class AItemsGroup;
class USGameInstance;
class APickupBase;
class APickupWeapon;
class APickupWeaponAcc;
class APickupAmmo;
class APickupHealth;
class APickupBoost;
class APickupEquipment;




UCLASS(minimalapi)
class APUBAdvTutGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APUBAdvTutGameMode();

	
	void GenerateItems();
	void GenrateItems();

	int32 RandomItemNumber();

	int32 RandomLocationNo();

	int32 RandomItemNo();



	E_ItemType RandomItemType();

	void RandomItemID(E_ItemType ItemTypex1,E_ItemType &RetItemType,FName &RetName);

	FTransform RandomLocation(TArray<FTransform> Available);


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

	FString TypesnIDTablePath;

	UDataTable* TypesnIDTableObject;

	USGameInstance* GameInstanceRef;

	int32 NoOfLocations=0;

	int32 NoOfItems=0;


protected:

	virtual void BeginPlay() override;

};



