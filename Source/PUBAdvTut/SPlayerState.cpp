// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "ItemBase.h"
#include "ItemWeapon.h"
#include "ItemAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "SGameInstance.h"

ASPlayerState::ASPlayerState(){
    ItemEquipmentTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ItemEquipment.DT_ItemEquipment'");
    ItemEquipmentTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemEquipmentTablePath));	
       
}

void ASPlayerState::BeginPlay()
{
	Super::BeginPlay();

    GameInstanceRef=Cast<USGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
}


AItemWeapon* ASPlayerState:: GetWeapon1(){
    return Weapon1;
}
AItemWeapon* ASPlayerState:: GetWeapon2(){
    return Weapon2;
}

AItemWeapon* ASPlayerState:: GetHoldGun(){
    return HoldGun;
}

int32 ASPlayerState::GetAmmo556(){
    return Ammo556;
}

int32 ASPlayerState::GetAmmo762(){
    return Ammo762;
}



float ASPlayerState::GetHealthPoint(){
    return HealthPoint;
}

float ASPlayerState::GetEnergyPoint(){
    return EnergyPoint;
}

TArray<AItemBase*> ASPlayerState::GetEquipments(){
    return Equipments;
}


TArray<AItemBase*> ASPlayerState::GetItemsInBackpack(){
    return ItemsInBackpack;
}

//Setters
void ASPlayerState::SetWeapon1(AItemWeapon* Weapon){
    Weapon1=Weapon;
    OnWeaponChanged.Broadcast(Weapon1,E_WeaponPosition::EWP_Left,false);
}

void ASPlayerState::SetWeapon2(AItemWeapon* Weapon){
    Weapon2=Weapon;
    OnWeaponChanged.Broadcast(Weapon2,E_WeaponPosition::EWP_Right,false);

}

void ASPlayerState::SetHoldGun(AItemWeapon* Weapon){
    HoldGun=Weapon;
    if(HoldGun){
        OnWeaponChanged.Broadcast(HoldGun,HoldGun->Position,true);
    }
    else{
        OnWeaponChanged.Broadcast(HoldGun,E_WeaponPosition::EWP_Left,true);
    }
}

void ASPlayerState::SetAmmo556(int32 Ammo556x){
    Ammo556 = Ammo556x;
    //UpdateAmmoObject();
    OnAmmoChanged.Broadcast(true);
}

void ASPlayerState::SetAmmo762(int32 Ammo762x){
    Ammo762 = Ammo762x;
    //UpdateAmmoObject();
    OnAmmoChanged.Broadcast(true);
}


void ASPlayerState::SetHealthPoint(float HP){
    HealthPoint=HP;
    OnHealthChanged.Broadcast(HealthPoint);
}

void ASPlayerState::SetEnergyPoint(float EP){
    EnergyPoint=EP;
    OnEnergyChanged.Broadcast(EnergyPoint);
}
//ArrayAdd

void ASPlayerState::AddEquipment(AItemBase* Equipment){
    Equipments.Add(Equipment);
    OnEquipmentChanged.Broadcast(Equipment,true);
}

void ASPlayerState::AddItemsInBackpack(AItemBase* Item){
    ItemsInBackpack.Add(Item);
    OnItemChanged.Broadcast(Item,true);
}

//ArrayRemove

bool ASPlayerState::RemoveEquipment(AItemBase* Equipment){
    Equipments.Remove(Equipment);
    OnEquipmentChanged.Broadcast(Equipment,false);
    return true;
}

bool ASPlayerState::RemoveItemsInBackpack(AItemBase* Item){
    ItemsInBackpack.Remove(Item);
    OnItemChanged.Broadcast(Item,false);
    return true;
}





bool ASPlayerState::CheckBackpackCapacity(int32 AddWeight){
    
    int32 TotalCapacity=GameInstanceRef->DefaultCapacity;
    int32 ItemsWeight=0;
    for(int32 i=0;i<Equipments.Num();i++){
        if(Equipments[i]->ItemType==E_ItemType::EIT_Backpack){
            ItemEquipmentRow=ItemEquipmentTableObject->FindRow<FSTR_ItemEquipment>(Equipments[i]->ID, TEXT(""));
            TotalCapacity+=ItemEquipmentRow->Capacity;
        }
    }

    for(int32 i=0;i<ItemsInBackpack.Num();i++){
        ItemsWeight+=ItemsInBackpack[i]->GetWeight();
    }
    if(AddWeight+ItemsWeight<=TotalCapacity){
        return true;
    }
    else{
        return false;
    }
    return false;
}







void ASPlayerState::UpdateAmmoObject(){
    AItemBase* Ammo1;
    AItemBase* Ammo2;
    int32 Ammo1Amt=GetAmmo556();
    int32 Ammo2Amt=GetAmmo762();
    for(int32 i=0;i<ItemsInBackpack.Num();i++){
        if(ItemsInBackpack[i]->ItemType==E_ItemType::EIT_Ammo){
            if(ItemsInBackpack[i]->ID==TEXT("1")){
                Ammo1=ItemsInBackpack[i];
            }
            else if(ItemsInBackpack[i]->ID==TEXT("2")){
                Ammo2=ItemsInBackpack[i];
            }
        }    
    }
    //Ammo1
    if(Ammo1){

        
        if(Ammo1Amt>0){
            if(Ammo1){
                Ammo1->Amount=Ammo1Amt;
            }
            else{
                AItemAmmo* TempAmmo;
                FTransform TempTransform;
                TempTransform.SetLocation(FVector::ZeroVector);
                TempTransform.SetRotation(FQuat(0,0,0,0));
                TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
                TempAmmo=GetWorld()->SpawnActorDeferred<AItemAmmo>(AItemAmmo::StaticClass(),TempTransform);
                if(TempAmmo){
                    
                    
                    TempAmmo->ID=TEXT("1");
                    
                    TempAmmo->SN=GameInstanceRef->GenerateSN();
                    TempAmmo->Amount=Ammo1Amt;	
                    
                    
                    UGameplayStatics::FinishSpawningActor(TempAmmo,TempTransform);
                }
                AItemBase* TempItemBase=Cast<AItemBase>(TempAmmo);
                AddItemsInBackpack(TempItemBase);
            }
            
        }

        else{
            if(Ammo1){
                
                RemoveItemsInBackpack(Ammo1);
                Ammo1->Destroy();
            
            }
        }
    }
    //Ammo2
    if(Ammo2){

    
        if(Ammo2Amt>0){
            if(Ammo2){
                Ammo2->Amount=Ammo2Amt;
            }
            else{
                AItemAmmo* TempAmmo;
                FTransform TempTransform;
                TempTransform.SetLocation(FVector::ZeroVector);
                TempTransform.SetRotation(FQuat(0,0,0,0));
                TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
                TempAmmo=GetWorld()->SpawnActorDeferred<AItemAmmo>(AItemAmmo::StaticClass(),TempTransform);
                if(TempAmmo){
                    
                    
                    TempAmmo->ID=TEXT("2");
                    
                    TempAmmo->SN=GameInstanceRef->GenerateSN();
                    TempAmmo->Amount=Ammo2Amt;	
                    
                    
                    UGameplayStatics::FinishSpawningActor(TempAmmo,TempTransform);
                }
                AItemBase* TempItemBase=Cast<AItemBase>(TempAmmo);
                AddItemsInBackpack(TempItemBase);
            }
            
        }
        else{
            if(Ammo2){
                
                RemoveItemsInBackpack(Ammo2);
                Ammo2->Destroy();
            
            }
        }
    }


}












void ASPlayerState::UpdateAmmoAmount(FName IDx1,bool bIsAdd,int32 Amountx1){
    if(Amountx1==0) {
        if(IDx1==TEXT("1")){
            SetAmmo556(0);
        }
        else if(IDx1==TEXT("2")){
            SetAmmo762(0);
        }
    }
    else if(bIsAdd){
        if(IDx1==TEXT("1")){
            Amountx1+=GetAmmo556();
            SetAmmo556(Amountx1);
        }
        else if(IDx1==TEXT("2")){
            Amountx1+=GetAmmo762();
            SetAmmo762(Amountx1);
        }
    }
    else if(!bIsAdd){
        if(IDx1==TEXT("1")){
            Amountx1-=GetAmmo556();
            SetAmmo556(Amountx1);
        }
        else if(IDx1==TEXT("2")){
            Amountx1-=GetAmmo762();
            SetAmmo762(Amountx1);
        }
    }
}




bool ASPlayerState::CheckReplaceBackpack(AItemBase* Item){
    
    int32 ItemsWeight=0;
    int32 PickupCapacity=0;
    TArray<AItemBase*> ItemsInBag= GetItemsInBackpack();
    for(int32 i=0;i<ItemsInBag.Num();i++){
        ItemsWeight+= ItemsInBag[i]->GetWeight();
    }
    if(Item){
        ItemEquipmentRow=ItemEquipmentTableObject->FindRow<FSTR_ItemEquipment>(Item->ID, TEXT(""));
        if(ItemEquipmentRow){
            PickupCapacity= ItemEquipmentRow->Capacity; 
        }
        
    }
    else{
        PickupCapacity=0;
    }
    if(GameInstanceRef){
        if(PickupCapacity+GameInstanceRef->DefaultCapacity>=ItemsWeight){
            return true;
        }
    }
    
    return false;

}




void ASPlayerState::UpdateWeaponAcc(E_WeaponPosition Positionx1,E_WeaponAccType AccTypex1,AItemWeaponAcc* ItemWeaponAccx1){
    AItemWeapon* LocalWeapon=nullptr;
    bool bIsRemove=false;
    if(Positionx1==E_WeaponPosition::EWP_Left){
        if(Weapon1){
            LocalWeapon=Weapon1;

        }
        else{
            LocalWeapon=HoldGun;
        }
    }
    else if(Positionx1==E_WeaponPosition::EWP_Right){
        if(Weapon2){
            LocalWeapon=Weapon1;

        }
        else{
            LocalWeapon=HoldGun;
        }
    }
    if(AccTypex1==E_WeaponAccType::EWAT_Sight){
        LocalWeapon->UpdateSight(ItemWeaponAccx1);
    }
    else if(AccTypex1==E_WeaponAccType::EWAT_Muzzle){
        LocalWeapon->UpdateMuzzle(ItemWeaponAccx1);
    }
    else if(AccTypex1==E_WeaponAccType::EWAT_Foregrip){
        LocalWeapon->UpdateForegrip(ItemWeaponAccx1);
    }
    else if(AccTypex1==E_WeaponAccType::EWAT_Mag){
        LocalWeapon->UpdateMag(ItemWeaponAccx1);
    }
    else if(AccTypex1==E_WeaponAccType::EWAT_Buttstock){
        LocalWeapon->UpdateButtstock(ItemWeaponAccx1);
    }

    if(ItemWeaponAccx1){

    }
    else{
        bIsRemove=true;
        
    }
    OnWeaponAccChanged.Broadcast(LocalWeapon, bIsRemove, ItemWeaponAccx1, AccTypex1);
}

int32 ASPlayerState::GetAmmoAmount(FName IDx){

    if(IDx==TEXT("1")){
        return 40;
    }
    else if(IDx==TEXT("2")){
        return 40;
    }
    return 0;


}
