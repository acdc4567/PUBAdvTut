// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_EquipEnd_Prone.h"
#include "SCharacter.h"


void UAN_EquipEnd_Prone::Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation){
    if(MeshComp!=NULL&&MeshComp->GetOwner()!=NULL){
        ASCharacter* MyCharacterRef=Cast<ASCharacter>(MeshComp->GetOwner());
        if(MyCharacterRef!=NULL){
            MyCharacterRef->LogIt();
        }   
    }
}
