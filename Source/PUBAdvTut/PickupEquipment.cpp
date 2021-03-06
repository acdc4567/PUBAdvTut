// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupEquipment.h"

APickupEquipment::APickupEquipment(){
    ItemEquipmentTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemEquipment.DT_ItemEquipment'");
	ItemEquipmentTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemEquipmentTablePath));	

}


void APickupEquipment::OnConstruction(const FTransform& Transform){

    ItemEquipmentRow = nullptr;
    ItemEquipmentRow= ItemEquipmentTableObject->FindRow<FSTR_ItemEquipment>(ID, TEXT(""));
    EquipmentType=ItemEquipmentRow->Type;

    FName identifier = TEXT("SWITCH TO ");
    FText Prefix1 = FText::FromName(identifier);
    InitPickup(EquipmentType,ItemEquipmentRow->Name,Prefix1,ItemEquipmentRow->PickupMesh);
    if(EquipmentType==E_ItemType::EIT_Vest||EquipmentType==E_ItemType::EIT_Backpack){
        StaticMesh->SetWorldScale3D(FVector(1.f,.1f,1.f));
        StaticMesh->SetWorldRotation(FRotator(0,0,90.f));
    }

}