// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EquipEnd_Crouch.generated.h"

/**
 * 
 */
UCLASS()
class PUBADVTUT_API UAN_EquipEnd_Crouch : public UAnimNotify
{
	GENERATED_BODY()
	public:
	virtual void Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation) override;

};
