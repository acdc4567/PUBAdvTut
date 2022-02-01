// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPlayerController.h"


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

	


}


void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
























//ASCharacter
