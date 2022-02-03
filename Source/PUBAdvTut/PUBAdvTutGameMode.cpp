// Copyright Epic Games, Inc. All Rights Reserved.

#include "PUBAdvTutGameMode.h"
#include "PUBAdvTutCharacter.h"
#include "ItemsGroup.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

APUBAdvTutGameMode::APUBAdvTutGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	ItemsGLTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/ItemGeneration/DT_ItemsGroupLocation.DT_ItemsGroupLocation'");
	ItemsGLTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemsGLTablePath));	

	NumbersPBTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/ItemGeneration/DT_NumberProbability.DT_NumberProbability'");
	NumbersPBTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *NumbersPBTablePath));	

	ItemTypePBTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/ItemGeneration/DT_ItemTypeProbability.DT_ItemTypeProbability'");
	ItemTypePBTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemTypePBTablePath));	

	WeaponDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemWeapon.DT_ItemWeapon'");
	WeaponDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponDTablePath));	

	WeaponAccDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemWeaponAcc.DT_ItemWeaponAcc'");
	WeaponAccDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponAccDTablePath));	
	
	AmmoDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemAmmo.DT_ItemAmmo'");
	AmmoDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *AmmoDTablePath));	
	
	HealthDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemHealth.DT_ItemHealth'");
	HealthDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *HealthDTablePath));	
	
	BoostDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemBoost.DT_ItemBoost'");
	BoostDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *BoostDTablePath));	
	
	EquipmentDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemEquipment.DT_ItemEquipment'");
	EquipmentDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *EquipmentDTablePath));	


}



void APUBAdvTutGameMode::BeginPlay(){
	Super::BeginPlay();
	GenerateItems();
	
}


void APUBAdvTutGameMode::GenerateItems(){
	FSTR_ItemsGroupLocation* ItemsGroupRow = nullptr;
	TArray<E_ItemType> ItemsTypeArr;
	TArray<FSTR_ItemTypesAndID*> ItemsIDArr;


	
	TArray<FName> RowNamesx1;
	RowNamesx1=ItemsGLTableObject->GetRowNames();


	FSTR_ItemWeapon* WeaponDTableRowx1=nullptr;

	for(auto& name:RowNamesx1){
		
		FString ContextString;
		ItemsGroupRow = ItemsGLTableObject->FindRow<FSTR_ItemsGroupLocation>(name, ContextString);
		
		
		AItemsGroup* ItemGroupRef;
		FTransform TempTransform;
		TempTransform.SetLocation(ItemsGroupRow->Location);
		TempTransform.SetRotation(FQuat(0,0,0,0));
		TempTransform.SetScale3D(FVector(1));
		ItemGroupRef=GetWorld()->SpawnActorDeferred<AItemsGroup>(AItemsGroup::StaticClass(),TempTransform);
		if(ItemGroupRef){
		
			UGameplayStatics::FinishSpawningActor(ItemGroupRef,TempTransform);
		}
		for(int32 k=0;k<RandomItemNumber();k++){
			E_ItemType TempItemType= RandomItemType();
			if(ItemsTypeArr.Find(TempItemType)==-1){
				ItemsTypeArr.Add(TempItemType);

			}
			else{
				
			}
		}
		for(int32 l=0;l<ItemsTypeArr.Num();l++){
			ItemsIDArr.Add( RandomItemID(ItemsTypeArr[l]) );
		}

		ItemsTypeArr.Empty();

		for(int32 i=0;i<ItemsIDArr.Num();i++){
			if(ItemsIDArr[i]->Type==E_ItemType::EIT_Weapon){
				WeaponDTableRowx1=WeaponDTableObject->FindRow<FSTR_ItemWeapon>(ItemsIDArr[i]->ID, ContextString);
				FSTR_ItemTypesAndID* TempStruct=nullptr;
				TempStruct->Type=E_ItemType::EIT_Ammo;
				TempStruct->ID=WeaponDTableRowx1->UseAmmoID;
				ItemsIDArr.Add(TempStruct);
				for(int32 m=0;m<ItemsIDArr.Num();m++){
					if(ItemsIDArr[m]->Type!=E_ItemType::EIT_Weapon){
						ItemsIDArr.Remove(ItemsIDArr[m]);
						break;
					}
				}
			}
		}


		/*
		for(int32 l=0;l<ItemsTypeArr.Num();l++){
			
			if(ItemsTypeArr[l]==E_ItemType::EIT_Accessories){
				UE_LOG(LogTemp,Warning,TEXT("Accessory"));
			}
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Ammo){
				UE_LOG(LogTemp,Warning,TEXT("Ammo"));
			}
			
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Weapon){
				UE_LOG(LogTemp,Warning,TEXT("Weapon"));
			}
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Backpack){
				UE_LOG(LogTemp,Warning,TEXT("Backpack"));
			}
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Helmet){
				UE_LOG(LogTemp,Warning,TEXT("Helmet"));
			}
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Vest){
				UE_LOG(LogTemp,Warning,TEXT("Vest"));
			}
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Health){
				UE_LOG(LogTemp,Warning,TEXT("Health"));
			}
			else if(ItemsTypeArr[l]==E_ItemType::EIT_Boost){
				UE_LOG(LogTemp,Warning,TEXT("boost"));
			}
			
		}
		UE_LOG(LogTemp,Warning,TEXT("--------"));
		ItemsTypeArr.Empty();
		*/
		
		



	}
	
	
	
	
	
}

int32 APUBAdvTutGameMode::RandomItemNumber(){
	FSTR_NumberProbability* NumberPBRow=nullptr;
	TArray<int32> Arr;

	TArray<FName> RowNamesx1;
	RowNamesx1=NumbersPBTableObject->GetRowNames();

	for(auto& name:RowNamesx1){
		FString ContextString;
		NumberPBRow = NumbersPBTableObject->FindRow<FSTR_NumberProbability>(name,ContextString);
		for(int32 k=0;k<NumberPBRow->Percentage;k++){
			Arr.Add(NumberPBRow->Number);

		}
		
	}
	int32 RandomNox1=FMath::RandRange(0,99);
	return Arr[RandomNox1];



}

E_ItemType APUBAdvTutGameMode::RandomItemType(){
	FSTR_ItemTypeProbability* ItemTypePBRow=nullptr;
	TArray<E_ItemType> Arr;

	TArray<FName> RowNamesx1;
	RowNamesx1=ItemTypePBTableObject->GetRowNames();

	for(auto& name:RowNamesx1){
		FString ContextString;
		ItemTypePBRow = ItemTypePBTableObject->FindRow<FSTR_ItemTypeProbability>(name, ContextString);
		for(int32 k=0;k<ItemTypePBRow->Percentage;k++){
			Arr.Add(ItemTypePBRow->Type);

		}
		
	}
	int32 RandomNox1=FMath::RandRange(0,99);
	return Arr[RandomNox1];

}

FSTR_ItemTypesAndID* APUBAdvTutGameMode::RandomItemID(E_ItemType ItemTypex1){
	TArray<FSTR_ItemTypesAndID*> Arr;
	
	if(ItemTypex1==E_ItemType::EIT_Weapon){
		FSTR_ItemWeapon* ItemWeaponRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=WeaponDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemWeaponRow = WeaponDTableObject->FindRow<FSTR_ItemWeapon>(name, ContextString);
			for(int32 i=0;i<ItemWeaponRow->ProbabilityPercent;i++){
				FSTR_ItemTypesAndID* TempStruct=nullptr;
				TempStruct->Type=E_ItemType::EIT_Weapon;
				TempStruct->ID=name;
				Arr.Add(TempStruct);
			}
		}
	}

	

	else if(ItemTypex1==E_ItemType::EIT_Accessories){
		FSTR_ItemWeaponAcc* ItemWeaponAccRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=WeaponAccDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemWeaponAccRow = WeaponAccDTableObject->FindRow<FSTR_ItemWeaponAcc>(name, ContextString);
			
			FSTR_ItemTypesAndID* TempStruct=nullptr;
			TempStruct->Type=E_ItemType::EIT_Accessories;
			TempStruct->ID=name;
			Arr.Add(TempStruct);
			
		}
	}

	else if(ItemTypex1==E_ItemType::EIT_Ammo){
		FSTR_ItemAmmo* ItemAmmoRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=WeaponDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemAmmoRow = AmmoDTableObject->FindRow<FSTR_ItemAmmo>(name, ContextString);
			for(int32 i=0;i<ItemAmmoRow->ProbabilityPercent;i++){
				FSTR_ItemTypesAndID* TempStruct=nullptr;
				TempStruct->Type=E_ItemType::EIT_Ammo;
				TempStruct->ID=name;
				Arr.Add(TempStruct);
			}
		}
	}


	else if(ItemTypex1==E_ItemType::EIT_Health){
		FSTR_ItemHealth* ItemHealthRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=HealthDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemHealthRow = HealthDTableObject->FindRow<FSTR_ItemHealth>(name, ContextString);
			for(int32 i=0;i<ItemHealthRow->ProbabilityPercent;i++){
				FSTR_ItemTypesAndID* TempStruct=nullptr;
				TempStruct->Type=E_ItemType::EIT_Health;
				TempStruct->ID=name;
				Arr.Add(TempStruct);
			}
		}
	}


	else if(ItemTypex1==E_ItemType::EIT_Boost){
		FSTR_ItemBoost* ItemBoostRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=BoostDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemBoostRow = BoostDTableObject->FindRow<FSTR_ItemBoost>(name, ContextString);
			for(int32 i=0;i<ItemBoostRow->ProbabilityPercent;i++){
				FSTR_ItemTypesAndID* TempStruct=nullptr;
				TempStruct->Type=E_ItemType::EIT_Boost;
				TempStruct->ID=name;
				Arr.Add(TempStruct);
			}
		}
	}

	else if(ItemTypex1==E_ItemType::EIT_Helmet){
		FSTR_ItemEquipment* ItemEquipmentRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=EquipmentDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemEquipmentRow = EquipmentDTableObject->FindRow<FSTR_ItemEquipment>(name, ContextString);
			if(ItemEquipmentRow->Type==E_ItemType::EIT_Helmet){

				for(int32 i=0;i<ItemEquipmentRow->ProbabilityPercent;i++){
					FSTR_ItemTypesAndID* TempStruct=nullptr;
					TempStruct->Type=E_ItemType::EIT_Helmet;
					TempStruct->ID=name;
					Arr.Add(TempStruct);
				}
			}
		}
	}

	else if(ItemTypex1==E_ItemType::EIT_Vest){
		FSTR_ItemEquipment* ItemEquipmentRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=EquipmentDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemEquipmentRow = EquipmentDTableObject->FindRow<FSTR_ItemEquipment>(name, ContextString);
			if(ItemEquipmentRow->Type==E_ItemType::EIT_Vest){

				for(int32 i=0;i<ItemEquipmentRow->ProbabilityPercent;i++){
					FSTR_ItemTypesAndID* TempStruct=nullptr;
					TempStruct->Type=E_ItemType::EIT_Vest;
					TempStruct->ID=name;
					Arr.Add(TempStruct);
				}
			}
		}
	}

	else if(ItemTypex1==E_ItemType::EIT_Backpack){
		FSTR_ItemEquipment* ItemEquipmentRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=EquipmentDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemEquipmentRow = EquipmentDTableObject->FindRow<FSTR_ItemEquipment>(name, ContextString);
			if(ItemEquipmentRow->Type==E_ItemType::EIT_Backpack){

				for(int32 i=0;i<ItemEquipmentRow->ProbabilityPercent;i++){
					FSTR_ItemTypesAndID* TempStruct=nullptr;
					TempStruct->Type=E_ItemType::EIT_Backpack;
					TempStruct->ID=name;
					Arr.Add(TempStruct);
				}
			}
		}
	}
	
	
	
	int32 RandomNox1=FMath::RandRange(0,Arr.Num());
	return Arr[RandomNox1];
}















//APUBAdvTutGameMode
