// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_UnEquip_Prone.h"
#include "SCharacter.h"


void UAN_UnEquip_Prone::Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation){
    if(MeshComp!=NULL&&MeshComp->GetOwner()!=NULL){
        ASCharacter* MyCharacterRef=Cast<ASCharacter>(MeshComp->GetOwner());
        if(MyCharacterRef!=NULL){
            MyCharacterRef->TakeBackWeapon();
        }   
    }
}
