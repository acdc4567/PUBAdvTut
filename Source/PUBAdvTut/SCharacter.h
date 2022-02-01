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

	FRotator GetControllerxRotation();

	void LogIt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	


private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterPose, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterPose, meta = (AllowPrivateAccess = "true"))
	bool bIsProne=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterPose, meta = (AllowPrivateAccess = "true"))
	bool bIsDead=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsHoldWeapon=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsFiring=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsEquip=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsReload=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsOnTheAir=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsFreeFalling=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsUmbrellaOpened=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OnTheAir, meta = (AllowPrivateAccess = "true"))
	float ForwardValue=0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsPlayingMontage=0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* inController) override;


	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE bool GetIsCrouching() const { return bIsCrouching; }
	FORCEINLINE bool GetIsProne() const { return bIsProne; }
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE bool GetIsHoldWeapon() const { return bIsHoldWeapon; }
	FORCEINLINE bool GetIsFiring() const { return bIsFiring; }
	FORCEINLINE bool GetIsEquip() const { return bIsEquip; }
	FORCEINLINE bool GetIsReload() const { return bIsReload; }
	FORCEINLINE bool GetIsOnTheAir() const { return bIsOnTheAir; }
	FORCEINLINE bool GetIsFreeFalling() const { return bIsFreeFalling; }
	FORCEINLINE bool GetIsUmbrellaOpened() const { return bIsUmbrellaOpened; }
	FORCEINLINE bool GetIsPlayingMontage() const { return bIsPlayingMontage; }
	FORCEINLINE float GetForwardValue() const { return ForwardValue; }















};
