// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWeaponAcc.h"
#include "Kismet/GameplayStatics.h"

APickupWeaponAcc::APickupWeaponAcc(){
    ItemWeaponAccTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemWeaponAcc.DT_ItemWeaponAcc'");
	ItemWeaponAccTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemWeaponAccTablePath));	

}

void APickupWeaponAcc::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}

int32 APickupWeaponAcc::GetWeight(){
    ItemWeaponAccRow = nullptr;
    ItemWeaponAccRow= ItemWeaponAccTableObject->FindRow<FSTR_ItemWeaponAcc>(ID, TEXT(""));
    return ItemWeaponAccRow->Weight;
}

void APickupWeaponAcc::OnConstruction(const FTransform& Transform){
    ItemWeaponAccRow = nullptr;
    ItemWeaponAccRow= ItemWeaponAccTableObject->FindRow<FSTR_ItemWeaponAcc>(ID, TEXT(""));

    FName identifier = TEXT("PICK UP ");
    FText Prefix1 = FText::FromName(identifier);
    AccType=ItemWeaponAccRow->Type;
    
    InitPickup(E_ItemType::EIT_Accessories,ItemWeaponAccRow->Name,Prefix1,ItemWeaponAccRow->StaticMesh);
    if(AccType==E_WeaponAccType::EWAT_Foregrip||AccType==E_WeaponAccType::EWAT_Mag||AccType==E_WeaponAccType::EWAT_Buttstock){
        StaticMesh->SetWorldRotation(FRotator(0,0,90.f));
    }

}
