// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ItemWeapon.h"
#include "ItemHealth.h"
#include "ItemBoost.h"
#include "ItemEquipment.h"
#include "PickupBase.h"
#include "PickupWeapon.h"
#include "PickupWeaponAcc.h"
#include "PickupAmmo.h"
#include "PickupBoost.h"
#include "PickupEquipment.h"
#include "PickupHealth.h"
#include "SPlayerState.h"
#include "SGameInstance.h"
#include "SPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"



ASCharacter::ASCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.f; 
	CameraBoom->bUsePawnControlRotation = true; 

	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	FPSCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);
	FPSCamera->bUsePawnControlRotation = true; 

	FPSArms=CreateDefaultSubobject<USkeletalMeshComponent>("FPSArms");
	FPSArms->SetupAttachment(FPSCamera);

	ArmsLocationTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ArmsLocation.DT_ArmsLocation'");
	ArmsLocationTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ArmsLocationTablePath));	


}


void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	if(Controller!=nullptr){
		PlayerStateRef=Cast<ASPlayerState>(Controller->PlayerState);

		
	}

	GameInstanceRef=Cast<USGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));



}


void ASCharacter::MouseTurn(float AxisValue){
	float TurnScaleFactor{};
	if (bIsAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(AxisValue * TurnScaleFactor);


}

void ASCharacter::MouseLookup(float AxisValue){

	float LookupScaleFactor{};
	if (bIsAiming)
	{
		LookupScaleFactor = MouseAimingLookupRate;
	}
	else
	{
		LookupScaleFactor = MouseHipLookupRate;
	}
	AddControllerPitchInput(AxisValue * LookupScaleFactor);



}

void ASCharacter::UpdateWeaponDisplay(FName HoldSocket){
	if(!bIsSightAiming){

	
		if(HoldSocket!=TEXT("N")){											//!bIsSightAiming

		
			if(HoldSocket!=TEXT("None")){

				if(PlayerStateRef->GetHoldGun()){
					const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(HoldSocket);
					const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
					PlayerStateRef->GetHoldGun()->SetActorTransform(SocketTransform);
					PlayerStateRef->GetHoldGun()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),HoldSocket);

				}
			}
			bool bIsEquipBackpack;


			TArray<AItemBase*> GetEquipmentsx1=PlayerStateRef->GetEquipments();
			for(int32 i=0;i<GetEquipmentsx1.Num();i++){
				if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Backpack){
					bIsEquipBackpack=true;
				}
			}


			if(PlayerStateRef->GetWeapon1()){
				if(bIsEquipBackpack){
					const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackLeftBName);
					const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
					PlayerStateRef->GetWeapon1()->SetActorTransform(SocketTransform);
					PlayerStateRef->GetWeapon1()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackLeftBName);

				}
				else{
					const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackLeftNName);
					const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
					PlayerStateRef->GetWeapon1()->SetActorTransform(SocketTransform);
					PlayerStateRef->GetWeapon1()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackLeftNName);

				}
			}

			if(PlayerStateRef->GetWeapon2()){
				if(bIsEquipBackpack){
					const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackRightBName);
					const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
					PlayerStateRef->GetWeapon2()->SetActorTransform(SocketTransform);
					PlayerStateRef->GetWeapon2()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackRightBName);

				}
				else{
					const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackRightNName);
					const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
					PlayerStateRef->GetWeapon2()->SetActorTransform(SocketTransform);
					PlayerStateRef->GetWeapon2()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackRightNName);

				}
			}

		}
	}	
}

void ASCharacter::UpdateEquipmentDisplay(){
	TArray<AItemBase*> GetEquipmentsx1=PlayerStateRef->GetEquipments();
	for(int32 i=0;i<GetEquipmentsx1.Num();i++){
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Helmet){
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->HelmetName);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			GetEquipmentsx1[i]->SetActorTransform(SocketTransform);
			GetEquipmentsx1[i]->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->HelmetName);

		}
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Vest){
			const USkeletalMeshSocket* SKMSocket =GetMesh()->GetSocketByName(GameInstanceRef->VestName);
			const FTransform SocketTransform = SKMSocket->GetSocketTransform(GetMesh());
			GetEquipmentsx1[i]->SetActorTransform(SocketTransform);
			GetEquipmentsx1[i]->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->VestName);

		}
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Backpack){
			const USkeletalMeshSocket* SKMSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackpackName);
			const FTransform SocketTransform = SKMSocket->GetSocketTransform(GetMesh());
			GetEquipmentsx1[i]->SetActorTransform(SocketTransform);
			GetEquipmentsx1[i]->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackpackName);

		}
	}

	

}


void ASCharacter::PossessedBy(AController* inController){
	ASPlayerController* SPC=Cast<ASPlayerController>(inController);
	if(SPC){
		MyPlayerControllerRef=SPC;
		SPC->OnPossessx(this);

	}

}	

FRotator ASCharacter::GetControllerxRotation(){
	if(Controller!=nullptr){
		return Controller->GetControlRotation();
	}
	return FRotator::ZeroRotator;
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	

}


void ASCharacter::LogIt(){
	UE_LOG(LogTemp,Warning,TEXT("Notify"));
	
}

void ASCharacter::PlayMontage(E_MontageType MontageType,float PlayRatex1){
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	PlayingMontageType=MontageType;
	bIsPlayingMontage=true;
	if(bIsProne){
		switch (MontageType)
		{
			case E_MontageType::EMT_Equip:
				if (AnimInstance && ProneEquipMontage)
				{
					AnimInstance->Montage_Play(ProneEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_UnEquip:
				if (AnimInstance && ProneUnEquipMontage)
				{
					AnimInstance->Montage_Play(ProneUnEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_Reload:
				if (AnimInstance && ProneReloadMontage)
				{
					AnimInstance->Montage_Play(ProneReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_ReloadBullet:
				if (AnimInstance && ProneReloadMontage)
				{
					AnimInstance->Montage_Play(ProneReloadMontage,PlayRatex1);
					AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
				}
			break;
			case E_MontageType::EMT_Fire:
				if (AnimInstance && ProneFireMontage)
				{
					AnimInstance->Montage_Play(ProneFireMontage,0.466f);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;
			case E_MontageType::EMT_Use:
				if (AnimInstance && ProneUseMontage)
				{
					AnimInstance->Montage_Play(ProneUseMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;


			
			
		}
	}
	else if(bIsCrouching){
		switch (MontageType)
		{
			case E_MontageType::EMT_Equip:
				if (AnimInstance && CrouchEquipMontage)
				{
					AnimInstance->Montage_Play(CrouchEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_UnEquip:
				if (AnimInstance && CrouchUnEquipMontage)
				{
					AnimInstance->Montage_Play(CrouchUnEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_Reload:
				if (AnimInstance && CrouchReloadMontage)
				{
					AnimInstance->Montage_Play(CrouchReloadMontage,PlayRatex1);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_ReloadBullet:
				if (AnimInstance && CrouchReloadMontage)
				{
					AnimInstance->Montage_Play(CrouchReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
				}
			break;
			case E_MontageType::EMT_Fire:
				if (AnimInstance && CrouchFireMontage)
				{
					AnimInstance->Montage_Play(CrouchFireMontage,2.f);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;
			case E_MontageType::EMT_Use:
				if (AnimInstance && CrouchUseMontage)
				{
					AnimInstance->Montage_Play(CrouchUseMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;


			
			
		}
	}
	else{
		switch (MontageType)
		{
			case E_MontageType::EMT_Equip:
				if (AnimInstance && EquipMontage)
				{
					AnimInstance->Montage_Play(EquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_UnEquip:
				if (AnimInstance && UnEquipMontage)
				{
					AnimInstance->Montage_Play(UnEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_Reload:
				if (AnimInstance && ReloadMontage)
				{
					AnimInstance->Montage_Play(ReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_ReloadBullet:
				if (AnimInstance && ReloadMontage)
				{
					AnimInstance->Montage_Play(ReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
				}
			break;
			case E_MontageType::EMT_Fire:
				if (AnimInstance && FireMontage)
				{
					AnimInstance->Montage_Play(FireMontage,2.f);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;
			case E_MontageType::EMT_Use:
				if (AnimInstance && UseMontage)
				{
					AnimInstance->Montage_Play(UseMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;


			
			
		}
	}


}


void ASCharacter::SwitchCamera(bool bIsFirst){

	FPSArms->SetVisibility(bIsFirst,true);
	UActorComponent* TempFPSCamera=Cast<UActorComponent>(FPSCamera);
	if(TempFPSCamera){
		TempFPSCamera->SetActive(bIsFirst,false);
	}
	GetMesh()->SetVisibility(!bIsFirst,true);
	UActorComponent* TempFollowCamera=Cast<UActorComponent>(FollowCamera);
	if(TempFollowCamera){
		TempFollowCamera->SetActive(!bIsFirst,false);
	}

	//NextStep
	if(PlayerStateRef->GetHoldGun()){
		AActor* TempHoldGun=Cast<AActor>(PlayerStateRef->GetHoldGun());
		
		if(bIsFirst){
			const USkeletalMeshSocket* FPSArmWeaponSocket=FPSArms->GetSocketByName("WeaponSocket");
			FTransform FPSArmWeaponSocketTransform ;
			if(FPSArmWeaponSocket){
				FPSArmWeaponSocketTransform = FPSArmWeaponSocket->GetSocketTransform(FPSArms);
				TempHoldGun->SetActorTransform(FPSArmWeaponSocketTransform);
				TempHoldGun->AttachToComponent(FPSArms,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("WeaponSocket"));
				PlayerStateRef->GetHoldGun()->SkeletalMesh->SetVisibility(true,true);

			}
		}
		else{
			FName TPPSocketName=MyPlayerControllerRef-> CalculateHoldGunSocket();
			const USkeletalMeshSocket* TPPWeaponSocket=GetMesh()->GetSocketByName(TPPSocketName);
			FTransform TPPWeaponSocketTransform ;
			if(TPPWeaponSocket){
				TPPWeaponSocketTransform = TPPWeaponSocket->GetSocketTransform(GetMesh());
				TempHoldGun->SetActorTransform(TPPWeaponSocketTransform);
				TempHoldGun->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TPPSocketName);
				PlayerStateRef->GetHoldGun()->SkeletalMesh->SetVisibility(true,true);

			}
		}
		//Next Steps

		if(PlayerStateRef->GetHoldGun()->SightAccActorx1){
			if(PlayerStateRef->GetHoldGun()->SightAccActorx1->ID==TEXT("2")){
				PlayerStateRef->GetHoldGun()->SightAccActorx1->SetActorRelativeScale3D(FVector(.1f,1.f,1.f));
				FPSCamera->SetFieldOfView(28.f);
				FPSArms->SetVisibility(false,false);
			}
			else{
				PlayerStateRef->GetHoldGun()->SightAccActorx1->SetActorRelativeScale3D(FVector(1.f,1.f,1.f));
				FPSCamera->SetFieldOfView(90.f);
			}
		}


		if(bIsFirst){
			FSTR_ArmsLocation* ArmsLocationRow=nullptr;
			FName TempAccID=TEXT("0");
			
			if(PlayerStateRef->GetHoldGun()->SightAccActorx1){
				TempAccID=PlayerStateRef->GetHoldGun()->SightAccActorx1->ID;

			}

			if(PlayerStateRef->GetHoldGun()->ID==TEXT("1")&&TempAccID==TEXT("0")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("1_0"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("2")&&TempAccID==TEXT("0")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("2_0"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("3")&&TempAccID==TEXT("0")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("3_0"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("1")&&TempAccID==TEXT("1")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("1_1"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("2")&&TempAccID==TEXT("1")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("2_1"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("3")&&TempAccID==TEXT("1")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("3_1"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("1")&&TempAccID==TEXT("2")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("1_2"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("2")&&TempAccID==TEXT("2")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("2_2"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			else if(PlayerStateRef->GetHoldGun()->ID==TEXT("3")&&TempAccID==TEXT("2")){
				ArmsLocationRow = ArmsLocationTableObject->FindRow<FSTR_ArmsLocation>(FName("3_2"), TEXT(""));
				FPSArms->SetRelativeLocation(ArmsLocationRow->Location);
			}
			



		}
   
	}

}















void ASCharacter::LoggedIt(bool bIsTrue){
	if(bIsTrue){
		UE_LOG(LogTemp,Warning,TEXT("DelegateRecieved"));
	}
	

}


void ASCharacter::SetIsPlayingMontage(bool Value){
	bIsPlayingMontage=Value;
}


void ASCharacter::TakeBackWeapon(){

	MyPlayerControllerRef->TakeBackWeapon();

}

void ASCharacter::EquipWeapon(){
	MyPlayerControllerRef->EquipWeapon();
}

void ASCharacter::PlayFPSFireMontage(){
	UAnimInstance* AnimInstancex = FPSArms->GetAnimInstance();
	if(AnimInstancex&&FPSArmFireMontage){
		AnimInstancex->Montage_Play(FPSArmFireMontage);
		AnimInstancex->Montage_JumpToSection(FName("Default"));
				
	}

}

void ASCharacter::PlayTPPFireMontage(){
	PlayMontage(E_MontageType::EMT_Fire);


}

void ASCharacter::SetIsSightAiming(bool Value){
	
	bIsSightAiming=Value; 
	if(bIsSightAiming){
		PlayerStateRef->GetHoldGun()->bCanPlayFiringFlash=0;
	}
	else{
		PlayerStateRef->GetHoldGun()->bCanPlayFiringFlash=1;
	}
}

void ASCharacter::PlayReloadxMontage(float PlayRatex1){
	PlayMontage(E_MontageType::EMT_Reload,PlayRatex1);
}

void ASCharacter::Event_ReloadEnd(){

	MyPlayerControllerRef->Event_ReloadEnd();

}

void ASCharacter::ExecuteReload(){
	MyPlayerControllerRef->ExecuteReload();
}

void ASCharacter::PlayReloadBulletMontage(){
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	PlayingMontageType=E_MontageType::EMT_ReloadBullet;
	bIsPlayingMontage=true;
	if(!bIsProne){
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
		}
	}
	else{
		if (AnimInstance && ProneReloadMontage)
		{
			AnimInstance->Montage_Play(ProneReloadMontage);
			AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
		}
	}
	

}


















//ASCharacter
