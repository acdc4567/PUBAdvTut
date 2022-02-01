// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class ASCharacter;

/**
 * 
 */
UCLASS()
class PUBADVTUT_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASPlayerController();



protected:
	virtual void BeginPlay() override;

	void MouseTurnx(float AxisValue);

	void MouseLookupx(float AxisValue);

	void MovingOnTheGround(bool bIsForward,float AxisValue);

	void MoveForwardKeyPressed(float AxisValue);

	void MoveRightKeyPressed(float AxisValue);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ASCharacter* MyCharacterRef;

public:
	void OnPossessx(ASCharacter* inCharacter);

	virtual void SetupInputComponent() override;
};
