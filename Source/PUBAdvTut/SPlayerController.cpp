// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "PickupBase.h"
#include "SGameInstance.h"
#include "SPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"


ASPlayerController::ASPlayerController(){

    ProneTimeTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_ProneTime.DT_ProneTime'");
	ProneTimeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ProneTimeTablePath));	

    WalkSpeedTablePath=TEXT("DataTable'/Game/Blueprints/Datas/DataTables/DT_WalkSpeed.DT_WalkSpeed'");
	WalkSpeedTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WalkSpeedTablePath));	


}


void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameInstanceRef=Cast<USGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	PlayerStateRef=Cast<ASPlayerState>(PlayerState);
    if(PlayerStateRef){
		PlayerStateRef->OnWeaponChanged.AddDynamic(this,&ASPlayerController::Event_WeaponChanged);
		PlayerStateRef->OnEquipmentChanged.AddDynamic(this,&ASPlayerController::Event_EquipmentChanged);
	}
	
}


void ASPlayerController::OnPossessx(ASCharacter* inCharacter){

    MyCharacterRef=inCharacter;


}

FName ASPlayerController::CalculateHoldGunSocket(){
	FName GunSocket;
	if(MyCharacterRef->GetIsProne()){
		if(MoveForwardAxis==0&&MoveRightAxis==0){
			GunSocket=GameInstanceRef->GunProneIdleName;
		}
		else if(MoveRightAxis==0){
			GunSocket=GameInstanceRef->GunProneFBName;
		}
		else{
			GunSocket=GameInstanceRef->GunProneOtherName;
		}
	}
	else if(MyCharacterRef->GetIsCrouching()&&!MyCharacterRef->GetIsAiming()){
		GunSocket=GameInstanceRef->GunCrouchName;
	}
	else if(MyCharacterRef->GetIsAiming()||MyCharacterRef->GetIsFiring()){
		GunSocket=GameInstanceRef->GunAimName;
	}
	else if(MyCharacterRef->GetIsReload()){
		GunSocket=GameInstanceRef->GunReloadName;
	}
	else{
		GunSocket=GameInstanceRef->GunStandName;
	}
	return GunSocket;
}


void ASPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(!bIsAltPressed){
        SetCharRotation();

    }
   



}


void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAxis("Turn", this, &ASPlayerController::MouseTurnx);
	InputComponent->BindAxis("LookUp", this, &ASPlayerController::MouseLookupx);

    InputComponent->BindAxis("MoveForward", this, &ASPlayerController::MoveForwardKeyPressed);
	InputComponent->BindAxis("MoveRight", this, &ASPlayerController::MoveRightKeyPressed);

    InputComponent->BindAction("AltKey",IE_Pressed ,this, &ASPlayerController::AltKeyPressed);
	InputComponent->BindAction("AltKey",IE_Released ,this, &ASPlayerController::AltKeyReleased);
	
    InputComponent->BindAction("Crouch",IE_Pressed ,this, &ASPlayerController::CrouchKeyPressed);
	
    InputComponent->BindAction("Prone",IE_Pressed ,this, &ASPlayerController::ProneKeyPressed);
	
    InputComponent->BindAction("Jump",IE_Pressed ,this, &ASPlayerController::JumpKeyPressed);
	
    InputComponent->BindAction("Walk",IE_Pressed ,this, &ASPlayerController::WalkKeyPressed);
	InputComponent->BindAction("Walk",IE_Released ,this, &ASPlayerController::WalkKeyReleased);
	
	InputComponent->BindAction("Interaction",IE_Pressed ,this, &ASPlayerController::InteractionKeyPressed);
	
	InputComponent->BindAction("Aiming",IE_Pressed ,this, &ASPlayerController::AimingKeyPressed);
	InputComponent->BindAction("Aiming",IE_Released ,this, &ASPlayerController::AimingKeyReleased);
	

}


void ASPlayerController::MouseTurnx(float AxisValue){
    if(LimitPitchAngle(AxisValue)){
        MyCharacterRef->MouseTurn(AxisValue);

    }

}

void ASPlayerController::MouseLookupx(float AxisValue){

    if(LimitPitchAngle(AxisValue)){
        MyCharacterRef->MouseLookup(AxisValue);

    }

}

void ASPlayerController::MovingOnTheGround(bool bIsForward,float AxisValue,FRotator ControllerRotationx){
    const FRotator Rotation{ ControllerRotationx };
    const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
    const FVector DirectionX{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
	const FVector DirectionY{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
    SmoothIncrease();
    if(bIsForward){
        if(bEnableMove){
            HandleWalkSpeedFromTable();
            MyCharacterRef->AddMovementInput(DirectionX,AxisValue);

        }
    }
    else{
        if(bEnableMove){
            HandleWalkSpeedFromTable();
            MyCharacterRef->AddMovementInput(DirectionY,AxisValue);

        }
    }


}

void ASPlayerController::MoveForwardKeyPressed(float AxisValue){
    if(MoveForwardAxis!=AxisValue){
        MoveForwardAxis=AxisValue;
        UpdateCameraHeight();
		MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

    }
    
    
    MovingOnTheGround(1,AxisValue,GetControllerxRotation());


}


void ASPlayerController::MoveRightKeyPressed(float AxisValue){
    if(MoveRightAxis!=AxisValue){
        MoveRightAxis=AxisValue;
        UpdateCameraHeight();
		MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());
    }
    
    
    MovingOnTheGround(0,AxisValue,GetControllerxRotation());


}

void ASPlayerController::SmoothIncrease(){

    FVector Velocity {MyCharacterRef-> GetCharacterMovement()->Velocity };
    Velocity.Z = 0;
    float Speed = Velocity.Size();
    Speed += 10.f;
    if (Speed <= WalkSpeed) {
       MyCharacterRef-> GetCharacterMovement()->MaxWalkSpeed = Speed;
    }
}

void ASPlayerController::SetCharRotation(){

    FRotator DeltaRotator=GetControlRotation();
    DeltaRotator.Roll=0;
    DeltaRotator.Pitch=0;
    const FQuat QDeltaRotator=DeltaRotator.Quaternion();
    bool bRotated=MyCharacterRef->SetActorRotation(QDeltaRotator); 

}


void ASPlayerController::AltKeyPressed(){
    bIsAltPressed=true;
    AltPressedRotation= MyCharacterRef->GetCameraBoom()->GetTargetRotation();


}

void ASPlayerController::AltKeyReleased(){
    
    AltReleasedRotation=MyCharacterRef->GetCameraBoom()->GetTargetRotation();
    CameraRestore(AltPressedRotation,AltReleasedRotation);
    bIsAltPressed=0;

}

FRotator ASPlayerController::GetControllerxRotation(){

    FRotator ReturnRotation=FRotator::ZeroRotator;
    if(bIsAltPressed){
        ReturnRotation=AltPressedRotation;
    }
    else{
        ReturnRotation=GetControlRotation();
    }


    return ReturnRotation;

}

void ASPlayerController::CrouchKeyPressed(){
    if(MyCharacterRef->GetIsProne()){
        MyCharacterRef->SetIsProne(0);
        MyCharacterRef->SetIsCrouching(1);
        HandleProneTimeFromTable(3,2);
    }
    else {
        if(MyCharacterRef->GetIsCrouching()){
            MyCharacterRef->SetIsCrouching(0);
        }
        else{
            MyCharacterRef->SetIsCrouching(1);

        }
    }
    
    LimitPitchAngle(0.f);
    UpdateCameraHeight();
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());


}




void ASPlayerController::ProneKeyPressed(){
    if(MyCharacterRef->GetIsProne()){
        MyCharacterRef->SetIsProne(0);
        HandleProneTimeFromTable(3,1);
    }
    else{
        if(MyCharacterRef->GetIsCrouching()){
            MyCharacterRef->SetIsCrouching(0);
            MyCharacterRef->SetIsProne(1);
            MyCharacterRef->SetIsAiming(0);
            HandleProneTimeFromTable(3,2);

        }
        else{
            MyCharacterRef->SetIsProne(1);
            MyCharacterRef->SetIsAiming(0);
            HandleProneTimeFromTable(1,3);
        }

    }
    
    LimitPitchAngle(0.f);
    UpdateCameraHeight();
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

}




void ASPlayerController::JumpKeyPressed(){
    if(MyCharacterRef->GetIsProne()){
        MyCharacterRef->SetIsProne(0);
        MyCharacterRef->SetIsCrouching(1);


    }
    else{
        if(MyCharacterRef->GetIsCrouching()){
            MyCharacterRef->SetIsCrouching(0);

        }
        else{
            MyCharacterRef->Jump();

        }
    }
    UpdateCameraHeight();

}


void ASPlayerController::HandleProneTimeFromTable(int32 Pose1,int32 Pose2){

//PoseNumber1=Stand,PoseNumber2=Crouch,PoseNumber3=Prone

	FSTR_ProneTime* ProneTimeRow = nullptr;
	if(Pose1==3&&Pose2==2){
		if(MyCharacterRef->GetIsHoldWeapon()){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_prone_crouch"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_prone_crouch"), TEXT(""));

		}
	}
	if(Pose1==3&&Pose2==1){
		if(MyCharacterRef->GetIsHoldWeapon()){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_prone_stand"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_prone_stand"), TEXT(""));

		}
	}
	if(Pose1==2&&Pose2==3){
		if(MyCharacterRef->GetIsHoldWeapon()){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_crouch_prone"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_crouch_prone"), TEXT(""));

		}
	}
	if(Pose1==1&&Pose2==3){
		if(MyCharacterRef->GetIsHoldWeapon()){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_stand_prone"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_stand_prone"), TEXT(""));

		}
	}
	if(ProneTimeRow){
		DisableMoveTime=ProneTimeRow->Sec;
		
	}


	bEnableMove=0;
	GetWorldTimerManager().SetTimer(TH_DisableMove,this,&ASPlayerController::EnableMovex,DisableMoveTime,false);


}

void ASPlayerController::EnableMovex(){

    bEnableMove=1;
}

bool ASPlayerController::LimitPitchAngle(float Axis){
	float BottomValue;
	float TopValue;
	if(MyCharacterRef->GetIsProne()){
		BottomValue=ProneBottomValue;
		TopValue=ProneTopValue;

	}
	else if(MyCharacterRef->GetIsCrouching() ){
		BottomValue=CrouchBottomValue;
		TopValue=CrouchTopValue;
	}
	else{
		BottomValue=StandBottomValue;
		TopValue=StandTopValue;
	}

	
    float ControllerPitch=GetControlRotation().Pitch;
    float ControllerYaw=GetControlRotation().Yaw;
    float ControllerRoll=GetControlRotation().Roll;
    
    float Excess=Axis*InputPitchScale;
    if(!(ControllerPitch>BottomValue||ControllerPitch<TopValue)){
        if(FMath::Abs(ControllerPitch-TopValue)<FMath::Abs(ControllerPitch-BottomValue)){
            LimitPitchAngleExcess(1,TopValue);
        }
        else
        {
           LimitPitchAngleExcess(0,BottomValue);

        }
        
    }
    
    if(ControllerPitch+Excess>BottomValue||ControllerPitch+Excess<TopValue){
        return true;
    }
    else{
        return false;
    }
	

	return true;
}



void ASPlayerController::UpdateCameraHeight(){

	if(MyCharacterRef->GetIsCrouching()){
		if(!(MyCharacterRef->GetIsHoldWeapon())&&(MoveForwardAxis!=0||MoveRightAxis!=0)){
			NewCameraHeight=CrouchRunCameraHeight;
		}
		else if((MyCharacterRef->GetIsHoldWeapon())&&(MoveForwardAxis!=0||MoveRightAxis!=0)){
			NewCameraHeight=CrouchRifleMoveCameraHeight;
		}
		else{
			NewCameraHeight=CrouchCameraHeight;
		}
	}
	else if(MyCharacterRef->GetIsProne()){
		NewCameraHeight=ProneCameraHeight;
	}
	else{
		NewCameraHeight=StandCameraHeight;
	}


	//NextSequence
	if(CurrentCameraHeight!=NewCameraHeight){
		OriginalCameraHeight= CurrentCameraHeight;
		HandleCameraHeightChanged(OriginalCameraHeight,NewCameraHeight);
	}



}

void ASPlayerController::WalkKeyPressed(){
    bWalkPressed=1;
    SmoothIncrease();

}

void ASPlayerController::WalkKeyReleased(){
    bWalkPressed=0;
    SmoothIncrease();
}

void ASPlayerController::RunKeyPressed(){
    bRunPressed=1;
    SmoothIncrease();
}

void ASPlayerController::RunKeyReleased(){
    bRunPressed=0;
    SmoothIncrease();
}

void ASPlayerController::HandleWalkSpeedFromTable(){
//PoseNumber1=Stand,PoseNumber2=Crouch,PoseNumber3=Prone
//WalkRun1=Walk,WalkRun2=Jog,WalkRun3=Run,WalkRun4=Aim

	FSTR_WalkSpeed* WalkSpeedRow = nullptr;
	if(MyCharacterRef->GetIsHoldWeapon()){
		if(MyCharacterRef->GetIsCrouching()){
			if(MyCharacterRef->GetIsAiming()){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_aim"), TEXT(""));

			}
			else if(bWalkPressed){
				
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_jog"), TEXT(""));

			}
			
		}
		else if(MyCharacterRef->GetIsProne()){
			if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_prone_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_prone_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_prone_jog"), TEXT(""));

			}

		}
		else{
			if(MyCharacterRef->GetIsAiming()){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_aim"), TEXT(""));

			}
			else if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_run"), TEXT(""));
				
			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_jog"), TEXT(""));

			}
		}
	}
	else{
		if(MyCharacterRef->GetIsCrouching()){
			
			if(bWalkPressed){
				
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_crouch_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_crouch_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_crouch_jog"), TEXT(""));

			}
			
		}
		else if(MyCharacterRef->GetIsProne()){
			if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_prone_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_prone_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_prone_jog"), TEXT(""));

			}

		}
		else{
			
			if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_stand_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_stand_run"), TEXT(""));
				
			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_stand_jog"), TEXT(""));

			}
		}
	}

	if(WalkSpeedRow){
		if(MoveForwardAxis>0){
			WalkSpeed=WalkSpeedRow->Forward;
		}
		else{
			WalkSpeed=WalkSpeedRow->Other;
		}
		
		
	}

	//WalkSpeed perWeapon
    /*
	if(PlayerStateRef->GetHoldGun()){
		
		WalkSpeed=WalkSpeed*PlayerStateRef->GetHoldGun()->ItemWeaponRow->WalkSpeedPer;
	}
    */
}

void ASPlayerController::InteractionKeyPressed(){

	

}

void ASPlayerController::Event_WeaponChanged(AItemWeapon * Weapon, E_WeaponPosition Position, bool bIsOnHand ){
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());


}

void ASPlayerController::Event_EquipmentChanged( AItemBase* Equipment,bool bIsAdd ){

	MyCharacterRef->UpdateEquipmentDisplay();
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

}

void ASPlayerController::AimingKeyPressed(){

	MyCharacterRef->SetIsAiming(1);
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

}

void ASPlayerController::AimingKeyReleased(){

	MyCharacterRef->SetIsAiming(0);
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

}































//ASPlayerController
