// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemWeaponAcc.h"
#include "ItemWeapon.generated.h"



class USkeletalMeshComponent;
class UStaticMeshComponent;
class ASPlayerState;
class UAudioComponent;
class ASCharacter;
class USGameInstance;
class ASPlayerController;
/**
 * 
 */
UCLASS()
class PUBADVTUT_API AItemWeapon : public AItemBase
{
	GENERATED_BODY()
	public:

	AItemWeapon();

	UFUNCTION(BlueprintCallable)
	void UpdateMag(AItemWeaponAcc* MagAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateMuzzle(AItemWeaponAcc* MuzzleAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateSight(AItemWeaponAcc* SightAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateForegrip(AItemWeaponAcc* ForegripAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateButtstock(AItemWeaponAcc* ButtstockAccActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	USkeletalMeshComponent* SkeletalMesh;

	void SwitchShootMode();

	void PressFire();

	void AutoFire();

	void ReleaseFire();

	int32 CheckAmmoAmount();

	void ReloadClip();

	void ChangeBullets();

	void FilledClip();


protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayFireSound();

	UFUNCTION(BlueprintCallable)
	void PlayFireFlash();

	


private:
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Muzzle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Foregrip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Buttstock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* FireFlash;


	FString ItemWeaponTablePath;

	UDataTable* ItemWeaponTableObject;

	float FireInterval;

	ASPlayerState* MyPlayerStateRef;

	USGameInstance* MyGameInstanceRef;

	ASPlayerController* MyPlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	bool bIsSightOpen=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	bool bNeedReloadBullet=0;

	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	int32 Ammo=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	E_WeaponPosition Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	bool bIsOnHand;

	FSTR_ItemWeapon* ItemWeaponRow;

	AItemWeaponAcc* MagAccActorx1;

	AItemWeaponAcc* MuzzleAccActorx1;

	AItemWeaponAcc* SightAccActorx1;

	AItemWeaponAcc* ForegripAccActorx1;

	AItemWeaponAcc* ButtstockAccActorx1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,meta=(ExposeOnSpawn="true"))
	E_ShootMode ShootModex1;

	FGate FireGate;

	FTimerHandle TH_FireTimerInProgress;

	bool bCanPlayFiringFlash=true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float FireTime;

	FORCEINLINE bool GetIsSightOpen() const { return bIsSightOpen; }
	
	FORCEINLINE bool GetNeedReloadBullet() const { return bNeedReloadBullet; }
	FORCEINLINE void SetNeedReloadBullet(bool Value) {  bNeedReloadBullet=Value; }
	

};
