

#include "PUBAdvTutGameMode.h"
#include "PUBAdvTutCharacter.h"
#include "ItemsGroup.h"
#include "Kismet/GameplayStatics.h"
#include "SGameInstance.h"
#include "PickupBase.h"
#include "PickupWeapon.h"
#include "PickupWeaponAcc.h"
#include "PickupAmmo.h"
#include "PickupHealth.h"
#include "PickupBoost.h"
#include "PickupEquipment.h"
#include "SPlayerController.h"
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

	TypesnIDTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemsTypesAndID.DT_ItemsTypesAndID'");
	TypesnIDTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *TypesnIDTablePath));	

}



void APUBAdvTutGameMode::BeginPlay(){
	Super::BeginPlay();

	GameInstanceRef=Cast<USGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PlayerControllerRef=Cast<ASPlayerController>(UGameplayStatics::GetPlayerController(this,0));
	
	if(PlayerControllerRef){
		PlayerControllerRef->SetPickupItems(GenrateItems());
	}
	
	
}





TArray<APickupBase*> APUBAdvTutGameMode::GenrateItems(){
	FSTR_ItemsGroupLocation* ItemsGroupRow = nullptr;
	TArray<APickupBase*> ItemsObject;
	TArray<FName> RowNamesx1;
	RowNamesx1=ItemsGLTableObject->GetRowNames();

	TArray<FTransform> SpawnTransformsx1;
	

	TArray<FTransform> UsedTransformsx1;

	for(FName& name:RowNamesx1){
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



		TArray<FVector> SpawnLocs=ItemGroupRef->GetAllLocations();
		for(int32 i=0;i<SpawnLocs.Num();i++){
			FTransform TempTransformx1;
			TempTransformx1.SetLocation(SpawnLocs[i]);
			TempTransformx1.SetRotation(FQuat(0,0,0,0));
			TempTransformx1.SetScale3D(FVector(1));
			++NoOfLocations;
			SpawnTransformsx1.Add(TempTransformx1);
		}
	}

	

	TArray<FName> RowNamesx2;
	RowNamesx2=TypesnIDTableObject->GetRowNames();
	FSTR_ItemTypesAndID* TypesnIDRow=nullptr;
	for(FName& name:RowNamesx2){
		++NoOfItems;
	}

	for(int32 i=0;i<60;i++){
		int32 TempRandInt=RandomLocationNo();
		if(TempRandInt<SpawnTransformsx1.Num()){

		
			FTransform TempTransformx3=SpawnTransformsx1[TempRandInt];
			FString ContextStringx1;
			int32 TempItemNo=RandomItemNo();
			FString TempString=FString::FromInt(TempItemNo);
			FName TempIDx1=FName(*TempString);
			FVector TempVector=TempTransformx3.GetLocation();
			TypesnIDRow=TypesnIDTableObject->FindRow<FSTR_ItemTypesAndID>(TempIDx1, ContextStringx1);
			if(TypesnIDRow){
				if(TempVector!=FVector::ZeroVector){

				
			
					if(TypesnIDRow->Type==E_ItemType::EIT_Weapon){
						APickupWeapon* TempPickupWeapon;
					
						TempPickupWeapon=GetWorld()->SpawnActorDeferred<APickupWeapon>(APickupWeapon::StaticClass(),TempTransformx3);
						if(TempPickupWeapon){
							TempPickupWeapon->ID=TypesnIDRow->ID;
							TempPickupWeapon->SN=GameInstanceRef->GenerateSN();
							TempPickupWeapon->Amount=1;
						
							UGameplayStatics::FinishSpawningActor(TempPickupWeapon,TempTransformx3);
						}
						APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupWeapon);
						ItemsObject.Add(TempPickupBase);
						
						



					}

					else if(TypesnIDRow->Type==E_ItemType::EIT_Accessories){
						APickupWeaponAcc* TempPickupWeaponAcc;
						
						TempPickupWeaponAcc=GetWorld()->SpawnActorDeferred<APickupWeaponAcc>(APickupWeaponAcc::StaticClass(),TempTransformx3);
						if(TempPickupWeaponAcc){
							TempPickupWeaponAcc->ID=TypesnIDRow->ID;
							TempPickupWeaponAcc->SN=GameInstanceRef->GenerateSN();
							TempPickupWeaponAcc->Amount=1;
						
							UGameplayStatics::FinishSpawningActor(TempPickupWeaponAcc,TempTransformx3);
						}
						APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupWeaponAcc);
						ItemsObject.Add(TempPickupBase);



					}

					else if(TypesnIDRow->Type==E_ItemType::EIT_Ammo){
						APickupAmmo* TempPickupAmmo;
						
						TempPickupAmmo=GetWorld()->SpawnActorDeferred<APickupAmmo>(APickupAmmo::StaticClass(),TempTransformx3);
						if(TempPickupAmmo){
							TempPickupAmmo->ID=TypesnIDRow->ID;
							TempPickupAmmo->SN=GameInstanceRef->GenerateSN();
							TempPickupAmmo->Amount=0;
						
							UGameplayStatics::FinishSpawningActor(TempPickupAmmo,TempTransformx3);
						}
						APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupAmmo);
						ItemsObject.Add(TempPickupBase);



					}

					else if(TypesnIDRow->Type==E_ItemType::EIT_Health){
						APickupHealth* TempPickupHealth;
						
						TempPickupHealth=GetWorld()->SpawnActorDeferred<APickupHealth>(APickupHealth::StaticClass(),TempTransformx3);
						if(TempPickupHealth){
							TempPickupHealth->ID=TypesnIDRow->ID;
							TempPickupHealth->SN=GameInstanceRef->GenerateSN();
							TempPickupHealth->Amount=1;
						
							UGameplayStatics::FinishSpawningActor(TempPickupHealth,TempTransformx3);
						}
						APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupHealth);
						ItemsObject.Add(TempPickupBase);



					}


					else if(TypesnIDRow->Type==E_ItemType::EIT_Boost){
						APickupBoost* TempPickupBoost;
						
						TempPickupBoost=GetWorld()->SpawnActorDeferred<APickupBoost>(APickupBoost::StaticClass(),TempTransformx3);
						if(TempPickupBoost){
							TempPickupBoost->ID=TypesnIDRow->ID;
							TempPickupBoost->SN=GameInstanceRef->GenerateSN();
							TempPickupBoost->Amount=1;
						
							UGameplayStatics::FinishSpawningActor(TempPickupBoost,TempTransformx3);
						}
						APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupBoost);
						ItemsObject.Add(TempPickupBase);



					}

					else if(TypesnIDRow->Type==E_ItemType::EIT_Helmet||TypesnIDRow->Type==E_ItemType::EIT_Vest||TypesnIDRow->Type==E_ItemType::EIT_Backpack){
						APickupEquipment* TempPickupEquipment;
						
						TempPickupEquipment=GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(),TempTransformx3);
						if(TempPickupEquipment){
							TempPickupEquipment->ID=TypesnIDRow->ID;
							TempPickupEquipment->SN=GameInstanceRef->GenerateSN();
							TempPickupEquipment->Amount=1;
						
							UGameplayStatics::FinishSpawningActor(TempPickupEquipment,TempTransformx3);
						}
						APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupEquipment);
						ItemsObject.Add(TempPickupBase);



					}
				}

			}
		}
	}
	
	return ItemsObject;

	




}






void APUBAdvTutGameMode::GenerateItems(){
	FSTR_ItemsGroupLocation* ItemsGroupRow = nullptr;
	TArray<E_ItemType> ItemsTypeArr;
	TArray<E_ItemType> ItemsEITArr;
	TArray<FName> ItemsIDArr;


	
	TArray<FName> RowNamesx1;
	RowNamesx1=ItemsGLTableObject->GetRowNames();


	FSTR_ItemWeapon* WeaponDTableRowx1=nullptr;
	E_ItemType SpawnType;
	FName SpawnID;
	TArray<FTransform> UsedLocations;
	FTransform SpawnTransformx2;
	TArray<APickupBase*> ItemsObject;

	for(FName& name:RowNamesx1){
		
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
				//continue;
			}
		}
		for(int32 l=0;l<ItemsTypeArr.Num();l++){
			
			E_ItemType TempEITx;
			FName TempNamex;
			RandomItemID(ItemsTypeArr[l],TempEITx,TempNamex);
			ItemsIDArr.Add( TempNamex );
			ItemsEITArr.Add(TempEITx);
		}

		ItemsTypeArr.Empty();
		/*
		for(int32 i=0;i<ItemsIDArr.Num();i++){
			if(ItemsEITArr[i]==E_ItemType::EIT_Weapon){
				WeaponDTableRowx1=WeaponDTableObject->FindRow<FSTR_ItemWeapon>(ItemsIDArr[i], ContextString);
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
		*/
		for(int32 i=0;i<ItemsIDArr.Num();i++){
			SpawnType=ItemsEITArr[i];
			SpawnID=ItemsIDArr[i];
			FTransform TempTransformx2;
			
			SpawnTransformx2=TempTransformx2;
			//UsedLocations.Add(TempTransformx2);
			


			if(SpawnType==E_ItemType::EIT_Weapon){
				APickupWeapon* TempPickupWeapon;
				
				TempPickupWeapon=GetWorld()->SpawnActorDeferred<APickupWeapon>(APickupWeapon::StaticClass(),SpawnTransformx2);
				if(TempPickupWeapon){
					TempPickupWeapon->ID=SpawnID;
					TempPickupWeapon->SN=GameInstanceRef->GenerateSN();
					TempPickupWeapon->Amount=1;
				
					UGameplayStatics::FinishSpawningActor(TempPickupWeapon,SpawnTransformx2);
				}
				APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupWeapon);
				ItemsObject.Add(TempPickupBase);



			}

			else if(SpawnType==E_ItemType::EIT_Accessories){
				APickupWeaponAcc* TempPickupWeaponAcc;
				
				TempPickupWeaponAcc=GetWorld()->SpawnActorDeferred<APickupWeaponAcc>(APickupWeaponAcc::StaticClass(),SpawnTransformx2);
				if(TempPickupWeaponAcc){
					TempPickupWeaponAcc->ID=SpawnID;
					TempPickupWeaponAcc->SN=GameInstanceRef->GenerateSN();
					TempPickupWeaponAcc->Amount=1;
				
					UGameplayStatics::FinishSpawningActor(TempPickupWeaponAcc,SpawnTransformx2);
				}
				APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupWeaponAcc);
				ItemsObject.Add(TempPickupBase);



			}

			else if(SpawnType==E_ItemType::EIT_Ammo){
				APickupAmmo* TempPickupAmmo;
				
				TempPickupAmmo=GetWorld()->SpawnActorDeferred<APickupAmmo>(APickupAmmo::StaticClass(),SpawnTransformx2);
				if(TempPickupAmmo){
					TempPickupAmmo->ID=SpawnID;
					TempPickupAmmo->SN=GameInstanceRef->GenerateSN();
					TempPickupAmmo->Amount=0;
				
					UGameplayStatics::FinishSpawningActor(TempPickupAmmo,SpawnTransformx2);
				}
				APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupAmmo);
				ItemsObject.Add(TempPickupBase);



			}

			else if(SpawnType==E_ItemType::EIT_Health){
				APickupHealth* TempPickupHealth;
				
				TempPickupHealth=GetWorld()->SpawnActorDeferred<APickupHealth>(APickupHealth::StaticClass(),SpawnTransformx2);
				if(TempPickupHealth){
					TempPickupHealth->ID=SpawnID;
					TempPickupHealth->SN=GameInstanceRef->GenerateSN();
					TempPickupHealth->Amount=1;
				
					UGameplayStatics::FinishSpawningActor(TempPickupHealth,SpawnTransformx2);
				}
				APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupHealth);
				ItemsObject.Add(TempPickupBase);



			}


			else if(SpawnType==E_ItemType::EIT_Boost){
				APickupBoost* TempPickupBoost;
				
				TempPickupBoost=GetWorld()->SpawnActorDeferred<APickupBoost>(APickupBoost::StaticClass(),SpawnTransformx2);
				if(TempPickupBoost){
					TempPickupBoost->ID=SpawnID;
					TempPickupBoost->SN=GameInstanceRef->GenerateSN();
					TempPickupBoost->Amount=1;
				
					UGameplayStatics::FinishSpawningActor(TempPickupBoost,SpawnTransformx2);
				}
				APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupBoost);
				ItemsObject.Add(TempPickupBase);



			}

			else if(SpawnType==E_ItemType::EIT_Helmet||SpawnType==E_ItemType::EIT_Vest||SpawnType==E_ItemType::EIT_Backpack){
				APickupEquipment* TempPickupEquipment;
				
				TempPickupEquipment=GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(),SpawnTransformx2);
				if(TempPickupEquipment){
					TempPickupEquipment->ID=SpawnID;
					TempPickupEquipment->SN=GameInstanceRef->GenerateSN();
					TempPickupEquipment->Amount=1;
				
					UGameplayStatics::FinishSpawningActor(TempPickupEquipment,SpawnTransformx2);
				}
				APickupBase* TempPickupBase=Cast<APickupBase>(TempPickupEquipment);
				ItemsObject.Add(TempPickupBase);



			}

		}

		ItemsIDArr.Empty();

		
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

	for(FName& name:RowNamesx1){
		FString ContextString;
		ItemTypePBRow = ItemTypePBTableObject->FindRow<FSTR_ItemTypeProbability>(name, ContextString);
		for(int32 k=0;k<ItemTypePBRow->Percentage;k++){
			Arr.Add(ItemTypePBRow->Type);

		}
		
	}
	int32 RandomNox1=FMath::RandRange(0,99);
	return Arr[RandomNox1];

}

void APUBAdvTutGameMode::RandomItemID(E_ItemType ItemTypex1,E_ItemType &RetItemType,FName &RetName){
	//TArray<FSTR_ItemTypesAndID*> Arr;
	
	TArray<E_ItemType> TempRetItemTypes;
	TArray<FName> TempRetNames;

	if(ItemTypex1==E_ItemType::EIT_Weapon){
		FSTR_ItemWeapon* ItemWeaponRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=WeaponDTableObject->GetRowNames();
		for(FName& name:RowNamesx1){
			FString ContextString;
			ItemWeaponRow = WeaponDTableObject->FindRow<FSTR_ItemWeapon>(name, ContextString);
			for(int32 i=0;i<ItemWeaponRow->ProbabilityPercent;i++){
				TempRetItemTypes.Add(E_ItemType::EIT_Weapon);
				TempRetNames.Add(name);


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
			
			TempRetItemTypes.Add(E_ItemType::EIT_Accessories);
			TempRetNames.Add(name);
			
		}
	}

	else if(ItemTypex1==E_ItemType::EIT_Ammo){
		FSTR_ItemAmmo* ItemAmmoRow=nullptr;
		
		
		TArray<FName> RowNamesx1;
		RowNamesx1=AmmoDTableObject->GetRowNames();
		for(auto& name:RowNamesx1){
			FString ContextString;
			ItemAmmoRow = AmmoDTableObject->FindRow<FSTR_ItemAmmo>(name, ContextString);
			for(int32 i=0;i<ItemAmmoRow->ProbabilityPercent;i++){
				TempRetItemTypes.Add(E_ItemType::EIT_Ammo);
				TempRetNames.Add(name);
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
				TempRetItemTypes.Add(E_ItemType::EIT_Health);
				TempRetNames.Add(name);
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
				TempRetItemTypes.Add(E_ItemType::EIT_Boost);
				TempRetNames.Add(name);
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
					TempRetItemTypes.Add(E_ItemType::EIT_Helmet);
					TempRetNames.Add(name);
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
					TempRetItemTypes.Add(E_ItemType::EIT_Vest);
					TempRetNames.Add(name);
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
					TempRetItemTypes.Add(E_ItemType::EIT_Backpack);
					TempRetNames.Add(name);
				}
			}
		}
	}
	
	
	
	int32 RandomNox1=FMath::RandRange(0,TempRetItemTypes.Num()-1);
	RetItemType=TempRetItemTypes[RandomNox1];
	RetName=TempRetNames[RandomNox1];
}



FTransform APUBAdvTutGameMode::RandomLocation(TArray<FTransform> Available){

	int32 RandomNox1=FMath::RandRange(0,Available.Num()-1);
	return Available[RandomNox1];
}



int32 APUBAdvTutGameMode::RandomItemNo(){
	int32 RandomNox1=FMath::RandRange(0,NoOfItems-1);
	return RandomNox1;
}



int32 APUBAdvTutGameMode::RandomLocationNo(){

	int32 RandomNox1=FMath::RandRange(0,NoOfLocations-1);
	return RandomNox1;

}





//APUBAdvTutGameMode
