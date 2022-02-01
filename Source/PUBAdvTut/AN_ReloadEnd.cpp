// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ReloadEnd.h"
#include "SCharacter.h"


void UAN_ReloadEnd::Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation){
    if(MeshComp!=NULL&&MeshComp->GetOwner()!=NULL){
        ASCharacter* MyCharacterRef=Cast<ASCharacter>(MeshComp->GetOwner());
        if(MyCharacterRef!=NULL){
            MyCharacterRef->LogIt();
        }   
    }
}
