// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "ItemBase.h"
#include "SPlayerController.generated.h"


class UBoxComponent;
class ASCharacter;
class APickupBase;
class USGameInstance;
class ASPlayerState;
class AItemWeapon;

USTRUCT(BlueprintType)
struct FSTR_ProneTime : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sec;


};

USTRUCT(BlueprintType)
struct FSTR_WalkSpeed : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Other;
	

	
};







/**
 * 
 */
UCLASS()
class PUBADVTUT_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASPlayerController();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CameraRestore(FRotator AltKeyPressedx,FRotator AltKeyReleasedx);

	UFUNCTION(BlueprintImplementableEvent)
	void LimitPitchAngleExcess(bool bGreater,float Value);

	TArray<APickupBase* > PickupItems;

	TArray<APickupBase* > ItemsInRange;
	
	UFUNCTION()
	void Event_WeaponChanged(AItemWeapon * Weapon, E_WeaponPosition Position, bool bIsOnHand );

	UFUNCTION()
	void Event_EquipmentChanged( AItemBase* Equipment,bool bIsAdd );

	void SetPickupItems(TArray<APickupBase*> Items);

	UFUNCTION()
	void UpdateCharacterGunState();

	void TakeBackWeapon();

	void EquipWeapon();

	FName CalculateHoldGunSocket();

protected:
	virtual void BeginPlay() override;

	void MouseTurnx(float AxisValue);

	void MouseLookupx(float AxisValue);

	void MovingOnTheGround(bool bIsForward,float AxisValue,FRotator ControllerRotationx);

	void MoveForwardKeyPressed(float AxisValue);

	void MoveRightKeyPressed(float AxisValue);

	void SmoothIncrease();

	void SetCharRotation();

	void AltKeyPressed();

	void AltKeyReleased();

	FRotator GetControllerxRotation();

	void CrouchKeyPressed();

	void ProneKeyPressed();

	void JumpKeyPressed();

	void HandleProneTimeFromTable(int32 Pose1,int32 Pose2);

	void EnableMovex();

	void UpdateCameraHeight();

	bool LimitPitchAngle(float Axis);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleCameraHeightChanged(float OriginalHeight,float NewHeight);
	
	void WalkKeyPressed();

	void WalkKeyReleased();

	void RunKeyPressed();

	void RunKeyReleased();

	void HandleWalkSpeedFromTable();

	

	void InteractionKeyPressed();

	void AimingKeyPressed();

	void AimingKeyReleased();

	void BeginPickupItem();

	void PickupWeapon(class APickupWeapon* PickupWeaponx1,bool bIsAssign,E_WeaponPosition Position1);
	
	bool PickupItem();

	UFUNCTION()
	void ExecBeginOverlap(APickupBase* PickupObject);

	UFUNCTION()
	void ExecEndOverlap(APickupBase* PickupObject);

	void OutlineItem(APickupBase* Item);

	void TargetingItem();

	//Items->PickupWeapon
	void AutoPosition(E_WeaponPosition &Positionx,bool &bIsOnHandx);

	void AssignPosition(const E_WeaponPosition Assign,E_WeaponPosition &Positionx,bool &bIsOnHandx);

	void SpawnPickupItem(AItemBase* ItemBasex1,APickupBase* &PickupItemx1);

	void CompleteSpawnPickupItem(APickupBase* PickupItemx1);

	//Items->PickupEquipment

	bool PickupEquipment(APickupBase* PickupBasex1);


	//Items->Discard

	void BeginDiscard();

	void DiscardWeapon(AItemWeapon* ItemWeaponx1);

	void TakeBackGunMontage();

	void Keyboard1KeyPressed();

	void Keyboard2KeyPressed();

	bool PickupGoods(APickupBase* PickupBasex1);
	
	
	void DiscardItem(AItemBase* Itemx1);

	bool DiscardEquipment(AItemBase* Itemx1,bool bIsCheck);

	bool EquipAccessories(AItemBase* ItemBasex1,bool bIsFromGround,AItemWeapon* Weaponx1);

	bool RemoveAccessories(AItemBase* ItemAccx1,bool bIsToGround,AItemWeapon* Weaponx1);

	void ReverseHoldAiming();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ASCharacter* MyCharacterRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed=600.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	bool bIsAltPressed=0;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	FRotator AltPressedRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	FRotator AltReleasedRotation;

	FString ProneTimeTablePath;

	UDataTable* ProneTimeTableObject;

	FString WalkSpeedTablePath;

	UDataTable* WalkSpeedTableObject;

	bool bEnableMove=1;

	float DisableMoveTime=0.f;

	FTimerHandle TH_DisableMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float ProneBottomValue=350.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float ProneTopValue=15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float CrouchBottomValue=290.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float CrouchTopValue=50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float StandBottomValue=290.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float StandTopValue=75.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float StandCameraHeight=80.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchCameraHeight=60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchRunCameraHeight=60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchRifleMoveCameraHeight=60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ProneCameraHeight=-20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CurrentCameraHeight=80.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float NewCameraHeight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float OriginalCameraHeight=80.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MoveForwardAxis=0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MoveRightAxis=0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bWalkPressed=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bRunPressed=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	AItemWeapon* ReadyEquipWeapon;

	USGameInstance* GameInstanceRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerState, meta = (AllowPrivateAccess = "true"))
	ASPlayerState* PlayerStateRef;

	APickupBase* ReadyPickupItem;

	bool bHoldAiming=0;

	float RightPressedTime=0.f;
	



public:
	void OnPossessx(ASCharacter* inCharacter);

	virtual void SetupInputComponent() override;


	
};
