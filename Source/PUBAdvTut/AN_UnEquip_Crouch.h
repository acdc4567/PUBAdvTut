// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_UnEquip_Crouch.generated.h"

/**
 * 
 */
UCLASS()
class PUBADVTUT_API UAN_UnEquip_Crouch : public UAnimNotify
{
	GENERATED_BODY()
	public:
	virtual void Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation) override;

};
