// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"


ASPlayerController::ASPlayerController(){

    


}


void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

    
	
}


void ASPlayerController::OnPossessx(ASCharacter* inCharacter){

    MyCharacterRef=inCharacter;


}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAxis("Turn", this, &ASPlayerController::MouseTurnx);
	InputComponent->BindAxis("LookUp", this, &ASPlayerController::MouseLookupx);

    InputComponent->BindAxis("MoveForward", this, &ASPlayerController::MoveForwardKeyPressed);
	InputComponent->BindAxis("MoveRight", this, &ASPlayerController::MoveRightKeyPressed);


}


void ASPlayerController::MouseTurnx(float AxisValue){

    MyCharacterRef->MouseTurn(AxisValue);

}

void ASPlayerController::MouseLookupx(float AxisValue){


    MyCharacterRef->MouseLookup(AxisValue);

}

void ASPlayerController::MovingOnTheGround(bool bIsForward,float AxisValue){
    const FRotator Rotation{ GetControlRotation() };
    const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
    const FVector DirectionX{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
	const FVector DirectionY{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
			
    if(bIsForward){
        MyCharacterRef->AddMovementInput(DirectionX,AxisValue);
    }
    else{
        MyCharacterRef->AddMovementInput(DirectionY,AxisValue);
    }


}

void ASPlayerController::MoveForwardKeyPressed(float AxisValue){
    MovingOnTheGround(1,AxisValue);


}


void ASPlayerController::MoveRightKeyPressed(float AxisValue){
    MovingOnTheGround(0,AxisValue);


}































//ASPlayerController
