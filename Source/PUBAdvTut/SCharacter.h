// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ASPlayerController;

UCLASS()
class PUBADVTUT_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	void MouseTurn(float AxisValue);

	void MouseLookup(float AxisValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	


private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MouseInput, meta = (AllowPrivateAccess = "true"))
	float MouseAimingTurnRate=.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MouseInput, meta = (AllowPrivateAccess = "true"))
	float MouseAimingLookupRate=.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MouseInput, meta = (AllowPrivateAccess = "true"))
	float MouseHipTurnRate=1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MouseInput, meta = (AllowPrivateAccess = "true"))
	float MouseHipLookupRate=1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MouseInput, meta = (AllowPrivateAccess = "true"))
	ASPlayerController* MyPlayerControllerRef;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* inController) override;


	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
