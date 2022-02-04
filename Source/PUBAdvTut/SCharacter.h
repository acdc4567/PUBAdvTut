// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemBase.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ASPlayerController;
class ASPlayerState;
class USkeletalMeshComponent;
class USGameInstance;
class AItemWeapon;
class APickupBase;

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

	UFUNCTION(BlueprintCallable,Category=ChangingWeaponDisplay)
	void UpdateWeaponDisplay(FName HoldSocket);

	UFUNCTION(BlueprintCallable,Category=ChangingEquipmentDisplay)
	void UpdateEquipmentDisplay();

	UFUNCTION()
	void LoggedIt(bool bIsTrue);

	

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	ASPlayerState* PlayerStateRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	USGameInstance* GameInstanceRef;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* inController) override;

//Getter
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


//Setter

	FORCEINLINE void SetIsProne(bool Value) {  bIsProne=Value; }
	FORCEINLINE void SetIsCrouching(bool Value) {  bIsCrouching=Value; }
	FORCEINLINE void SetIsAiming(bool Value) {  bIsAiming=Value; }
	FORCEINLINE void SetIsDead(bool Value) {  bIsDead=Value; }
	FORCEINLINE void SetIsHoldWeapon(bool Value) {  bIsHoldWeapon=Value; }
	FORCEINLINE void SetIsFiring(bool Value) {  bIsFiring=Value; }
	FORCEINLINE void SetIsEquip(bool Value) {  bIsEquip=Value; }
	FORCEINLINE void SetIsReload(bool Value) {  bIsReload=Value; }
	FORCEINLINE void SetIsOnTheAir(bool Value) {  bIsOnTheAir=Value; }
	FORCEINLINE void SetIsFreeFalling(bool Value) {  bIsFreeFalling=Value; }
	FORCEINLINE void SetIsUmbrellaOpened(bool Value) {  bIsUmbrellaOpened=Value; }
	
	UFUNCTION(BlueprintCallable)
	void SetIsPlayingMontage(bool Value);
	
	








};
