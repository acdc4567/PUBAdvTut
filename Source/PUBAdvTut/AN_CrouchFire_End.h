// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_CrouchFire_End.generated.h"

/**
 * 
 */
UCLASS()
class PUBADVTUT_API UAN_CrouchFire_End : public UAnimNotify
{
	GENERATED_BODY()
	public:
	virtual void Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation) override;

};
