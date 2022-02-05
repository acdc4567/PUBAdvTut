// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "ItemWeapon.h"
#include "ItemHealth.h"
#include "ItemBoost.h"
#include "ItemAmmo.h"
#include "ItemEquipment.h"
#include "PickupBase.h"
#include "PickupWeapon.h"
#include "PickupWeaponAcc.h"
#include "PickupAmmo.h"
#include "PickupBoost.h"
#include "PickupEquipment.h"
#include "PickupHealth.h"
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
   
	TargetingItem();



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
	
	InputComponent->BindAction("DiscardWeapon",IE_Pressed,this,&ASPlayerController::BeginDiscard);
	
	InputComponent->BindAction("Interaction",IE_Pressed,this,&ASPlayerController::BeginPickupItem);

	InputComponent->BindAction("TakeBackGun",IE_Pressed,this,&ASPlayerController::TakeBackGunMontage);

	InputComponent->BindAction("Keyboard1",IE_Pressed,this,&ASPlayerController::Keyboard1KeyPressed);
	
	InputComponent->BindAction("Keyboard2",IE_Pressed,this,&ASPlayerController::Keyboard2KeyPressed);

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
    if(!(MyCharacterRef->GetIsProne()&&MyCharacterRef->GetIsAiming())){
	
		if(MoveForwardAxis!=AxisValue){
			MoveForwardAxis=AxisValue;
			UpdateCameraHeight();
			MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

		}
		
		
		MovingOnTheGround(1,AxisValue,GetControllerxRotation());
	}

}


void ASPlayerController::MoveRightKeyPressed(float AxisValue){
    
	if(!(MyCharacterRef->GetIsProne()&&MyCharacterRef->GetIsAiming())){
	
		if(MoveRightAxis!=AxisValue){
			MoveRightAxis=AxisValue;
			UpdateCameraHeight();
			MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());
		}
		
		
		MovingOnTheGround(0,AxisValue,GetControllerxRotation());
	}

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
	ReverseHoldAiming();
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
		ReverseHoldAiming();
        HandleProneTimeFromTable(3,1);
    }
    else{
        if(MyCharacterRef->GetIsCrouching()){
            MyCharacterRef->SetIsAiming(0);
			MyCharacterRef->SetIsCrouching(0);
            MyCharacterRef->SetIsProne(1);
           
			ReverseHoldAiming();
			HandleProneTimeFromTable(3,2);

        }
        else{
			MyCharacterRef->SetIsAiming(0);
            MyCharacterRef->SetIsProne(1);
            ReverseHoldAiming();
            HandleProneTimeFromTable(1,3);
        }

    }
    
    LimitPitchAngle(0.f);
    UpdateCameraHeight();
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

}




void ASPlayerController::JumpKeyPressed(){
    if(!MyCharacterRef->GetIsAiming()){

	

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
    
	if(PlayerStateRef->GetHoldGun()){
		
		WalkSpeed=WalkSpeed*PlayerStateRef->GetHoldGun()->ItemWeaponRow->WalkSpeedPer;
	}
    
}

void ASPlayerController::InteractionKeyPressed(){

	

}

void ASPlayerController::Event_WeaponChanged(AItemWeapon * Weapon, E_WeaponPosition Position, bool bIsOnHand ){
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());
	UpdateCharacterGunState();

}

void ASPlayerController::Event_EquipmentChanged( AItemBase* Equipment,bool bIsAdd ){

	MyCharacterRef->UpdateEquipmentDisplay();
	MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());

}

void ASPlayerController::AimingKeyPressed(){
	if(!(MyCharacterRef->GetIsProne()&&(MoveForwardAxis!=0||MoveRightAxis!=0))&&!MyCharacterRef->GetIsSightAiming()&&(!MyCharacterRef->GetIsPlayingMontage()||MyCharacterRef->PlayingMontageType==E_MontageType::EMT_Fire)){
		RightPressedTime=GetWorld()->GetTimeSeconds();
		if(MyCharacterRef->GetIsHoldWeapon()){
			bHoldAiming=1;
			MyCharacterRef->SetIsAiming(1);
			MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());
			MyCharacterRef->HoldAiming(1);
		}
		if(PlayerStateRef->GetHoldGun()){
			//PlayerStateRef->GetHoldGun()->FireTime=0.f;
		}

	}
	
}

void ASPlayerController::AimingKeyReleased(){
	
	if(MyCharacterRef->GetIsHoldWeapon()){
		if(MyCharacterRef->GetIsSightAiming()){
			MyCharacterRef->SwitchCamera(0);
			MyCharacterRef->HoldAiming(0);
			MyCharacterRef->SetIsSightAiming(0);
		}
		else{
			if(GetWorld()->GetTimeSeconds()-RightPressedTime<.25f){
				MyCharacterRef->SetIsSightAiming(1);
				bHoldAiming=0;
			}
			else{
				MyCharacterRef->SetIsAiming(0);
				if(bHoldAiming){
					bHoldAiming=0;
					MyCharacterRef->HoldAiming(0);
					MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());
				}
			}


		}

	}
	
	
}

void ASPlayerController::ReverseHoldAiming(){
	
	if(MyCharacterRef->GetIsSightAiming()){
		MyCharacterRef->SwitchCamera(0);
		MyCharacterRef->HoldAiming(0);
		MyCharacterRef->SetIsSightAiming(0);

	}
	else{
		MyCharacterRef->SetIsAiming(0);
		if(bHoldAiming){
			bHoldAiming=0;
		
			MyCharacterRef->SetIsAiming(0);
			MyCharacterRef->HoldAiming(0);
			MyCharacterRef->UpdateWeaponDisplay(CalculateHoldGunSocket());
		}
	}
	
	
}


void ASPlayerController::SetPickupItems(TArray<APickupBase*> Items){
	PickupItems=Items;
	for(int32 i=0;i<PickupItems.Num();i++){
		PickupItems[i]->OnBeginOverlap.AddDynamic(this,&ASPlayerController::ExecBeginOverlap);
		PickupItems[i]->OnEndOverlap.AddDynamic(this,&ASPlayerController::ExecEndOverlap);
	
	}
}

void ASPlayerController::ExecBeginOverlap(APickupBase* PickupObject){
	
	//OutlineItem(PickupObject);
	ItemsInRange.Add(PickupObject);
}

void ASPlayerController::ExecEndOverlap(APickupBase* PickupObject){
	
	PickupObject->EnabledOutLine(false);
	ItemsInRange.Remove(PickupObject);
}

void ASPlayerController::OutlineItem(APickupBase* Item){
	for(int32 i=0;i<ItemsInRange.Num();i++){
		if(Item==ItemsInRange[i]){
			Item->EnabledOutLine(true);

		}
		else{
			Item->EnabledOutLine(false);
		}
	}
}

void ASPlayerController::TargetingItem(){
	FHitResult OutHitResult;
	FVector OutHitLocation;
	float ShortestDistance=10000.f;
	APickupBase* ShortestItem;


	if(ItemsInRange.Num()>0){
			// Get Viewport Size
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Get screen space location of crosshairs
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		// Get world position and direction of crosshairs
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection);

		if (bScreenToWorld){
			// Trace from Crosshair world location outward
			const FVector Start{ CrosshairWorldPosition };
			const FVector End{ Start + CrosshairWorldDirection * 500.f };
			OutHitLocation = End;
			GetWorld()->LineTraceSingleByChannel(
				OutHitResult,
				Start,
				End,
				ECollisionChannel::ECC_Visibility);
		}
		for(int32 i=0;i<ItemsInRange.Num();i++){
			ItemsInRange[i]->EnabledOutLine(false);
			if(FVector::Dist(ItemsInRange[i]->GetActorLocation(),OutHitResult.Location)<ShortestDistance){
				ShortestDistance=FVector::Dist(ItemsInRange[i]->GetActorLocation(),OutHitResult.Location);
				ShortestItem=ItemsInRange[i];
			}
		}
		if(ShortestItem){
			if(ShortestDistance<85.f)
			ShortestItem->EnabledOutLine(true);
			ReadyPickupItem=ShortestItem;

		}
		else{
			ReadyPickupItem=nullptr;
		}
	}

}

void ASPlayerController::AutoPosition(E_WeaponPosition &Positionx,bool &bIsOnHandx){
	
	int32 CurrentAmount=0;
	if(PlayerStateRef->GetWeapon1()){
		CurrentAmount++;

	}
	if(PlayerStateRef->GetWeapon2()){
		CurrentAmount++;

	}
	if(PlayerStateRef->GetHoldGun()){
		CurrentAmount++;
	}
	if(CurrentAmount==0){
		Positionx=E_WeaponPosition::EWP_Left;
		bIsOnHandx=true;
		
	}
	else if(CurrentAmount<2){
		if(PlayerStateRef->GetHoldGun()){
			if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Left){
				Positionx=E_WeaponPosition::EWP_Right;
				bIsOnHandx=false;
			}
			else if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Right){
				Positionx=E_WeaponPosition::EWP_Left;
				bIsOnHandx=false;
			}
		}
		else{
			if(PlayerStateRef->GetWeapon1()){
				Positionx=E_WeaponPosition::EWP_Right;
				bIsOnHandx=false;
			}
			else{
				Positionx=E_WeaponPosition::EWP_Left;
				bIsOnHandx=false;

			}
		}
	}
	else{
		if(PlayerStateRef->GetHoldGun()){
			if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Left){
				Positionx=E_WeaponPosition::EWP_Left;
				bIsOnHandx=true;
			}
			else if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Right){
				Positionx=E_WeaponPosition::EWP_Right;
				bIsOnHandx=true;
			}
		}
		else{
			Positionx=E_WeaponPosition::EWP_Left;
			bIsOnHandx=false;
		}
	}


}

void ASPlayerController::AssignPosition(const E_WeaponPosition Assign,E_WeaponPosition &Positionx,bool &bIsOnHandx){

	if(PlayerStateRef->GetHoldGun()){
		if(PlayerStateRef->GetHoldGun()->Position==Assign){
			Positionx=Assign;
			bIsOnHandx=true;
		}
		else{
			Positionx=Assign;
			bIsOnHandx=false;
		}
	}
	else{
		Positionx=Assign;
		bIsOnHandx=false;
	}

}

void ASPlayerController::SpawnPickupItem(AItemBase* ItemBasex1,APickupBase* &PickupItemx1){
	FName TempID=ItemBasex1->ID;
	FString TempSN=ItemBasex1->SN;
	FTransform TempTransform ;
	TempTransform.SetLocation(MyCharacterRef->GetActorLocation());
	TempTransform.SetRotation(FQuat(0,0,0,0));
	TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
	if(ItemBasex1->ItemType==E_ItemType::EIT_Weapon){
		APickupWeapon* TempPickupWeapon=Cast<APickupWeapon>(ItemBasex1);
		TempPickupWeapon=GetWorld()->SpawnActorDeferred<APickupWeapon>(APickupWeapon::StaticClass(),TempTransform);
		if(TempPickupWeapon){
		
			TempPickupWeapon->ID=TempID;
			TempPickupWeapon->SN=TempSN;
			TempPickupWeapon->Amount=1;	
			AItemWeapon* TempWeapon=Cast<AItemWeapon>(ItemBasex1);
			
			TempPickupWeapon->Ammo=TempWeapon->Ammo;
			UGameplayStatics::FinishSpawningActor(TempPickupWeapon,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupWeapon);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Accessories){
		APickupWeaponAcc* TempPickupWeapon=Cast<APickupWeaponAcc>(ItemBasex1);
		TempPickupWeapon=GetWorld()->SpawnActorDeferred<APickupWeaponAcc>(APickupWeaponAcc::StaticClass(),TempTransform);
		if(TempPickupWeapon){
			TempPickupWeapon->ID=TempID;
			TempPickupWeapon->SN=TempSN;
			TempPickupWeapon->Amount=1;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupWeapon,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupWeapon);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Ammo){
		APickupAmmo* TempPickupBase=Cast<APickupAmmo>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupAmmo>(APickupAmmo::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Health){
		APickupHealth* TempPickupBase=Cast<APickupHealth>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupHealth>(APickupHealth::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Boost){
		APickupBoost* TempPickupBase=Cast<APickupBoost>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupBoost>(APickupBoost::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Helmet||ItemBasex1->ItemType==E_ItemType::EIT_Vest||ItemBasex1->ItemType==E_ItemType::EIT_Backpack){
		APickupEquipment* TempPickupBase=Cast<APickupEquipment>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=1;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}
	
	
	CompleteSpawnPickupItem(PickupItemx1);
	
	
	

}




void ASPlayerController::CompleteSpawnPickupItem(APickupBase* PickupItemx1){
	PickupItems.Add(PickupItemx1);
	PickupItemx1->OnBeginOverlap.AddDynamic(this,&ASPlayerController::ExecBeginOverlap);
	PickupItemx1->OnEndOverlap.AddDynamic(this,&ASPlayerController::ExecEndOverlap);
	PickupItemx1->GetBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupItemx1->GetBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void ASPlayerController::BeginDiscard(){
	if(!MyCharacterRef->GetIsProne()&&!MyCharacterRef->GetIsSightAiming()){
		if(!MyCharacterRef->GetIsPlayingMontage()){
			if(PlayerStateRef->GetHoldGun()){
				DiscardWeapon(PlayerStateRef->GetHoldGun());
				ReverseHoldAiming();
			}
			else if(PlayerStateRef->GetWeapon1()){
				DiscardWeapon(PlayerStateRef->GetWeapon1());
			}
			else if (PlayerStateRef->GetWeapon2()){
				DiscardWeapon(PlayerStateRef->GetWeapon2());
			}
		}
	}
}

void ASPlayerController::DiscardWeapon(AItemWeapon* ItemWeaponx1){
	FName TempIDName;
	if(ItemWeaponx1->Ammo>0){
		TempIDName=ItemWeaponx1->ItemWeaponRow->UseAmmoID;
		AItemAmmo* TempAmmo;
		FTransform TempTransform;
		TempTransform.SetLocation(FVector::ZeroVector);
		TempTransform.SetRotation(FQuat(0,0,0,0));
		TempTransform.SetScale3D(FVector(1.f,1.f,1.f));

		TempAmmo=GetWorld()->SpawnActorDeferred<AItemAmmo>(AItemAmmo::StaticClass(),TempTransform);
		if(TempAmmo){
			
			TempAmmo->ID=TempIDName;
			TempAmmo->SN=GameInstanceRef->GenerateSN();
			TempAmmo->Amount=ItemWeaponx1->Ammo;	
			
			
			UGameplayStatics::FinishSpawningActor(TempAmmo,TempTransform);
		}
		APickupBase* TempPickupBasex1;
		SpawnPickupItem(TempAmmo,TempPickupBasex1);
		ItemWeaponx1->Ammo=0;
	}
	APickupBase* TempPickupBase;
	SpawnPickupItem(ItemWeaponx1,TempPickupBase);
	if (ItemWeaponx1->SightAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->SightAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->SightAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->ForegripAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->ForegripAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->ForegripAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->MagAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->MagAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->MagAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->MuzzleAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->MuzzleAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->MuzzleAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->ButtstockAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->ButtstockAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->ButtstockAccActorx1)->Destroy();
		
	}
	if(ItemWeaponx1->bIsOnHand){
		PlayerStateRef->SetHoldGun(nullptr);
	}
	else{
		if(ItemWeaponx1->Position==E_WeaponPosition::EWP_Left){
			PlayerStateRef->SetWeapon1(nullptr);
		}
		else if(ItemWeaponx1->Position==E_WeaponPosition::EWP_Right){
			PlayerStateRef->SetWeapon2(nullptr);

		}
	} 
	bool bIsDestroyed=Cast<AActor>(ItemWeaponx1)->Destroy();
		
}


void ASPlayerController::BeginPickupItem(){
	
	bool bIsSuccess=PickupItem();
}



bool ASPlayerController::PickupItem(){
	
	if(ReadyPickupItem){
		if(ReadyPickupItem->ItemType==E_ItemType::EIT_Weapon){
			APickupWeapon* TempWeapon=Cast<APickupWeapon>(ReadyPickupItem);
			bool TempbIsAssign=false;
			E_WeaponPosition TempPosition=E_WeaponPosition::EWP_Left;
			PickupWeapon(TempWeapon,TempbIsAssign,TempPosition);
			bool bIsSuccess=Cast<AActor>(ReadyPickupItem)->Destroy();
			return true;
		}


		
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Accessories){
			APickupWeaponAcc* TempWeaponAcc=Cast<APickupWeaponAcc>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempWeaponAcc);
			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx1=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;

			}
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Ammo){
			APickupAmmo* TempAmmo=Cast<APickupAmmo>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempAmmo);
			
			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx2=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;
				
			}
				
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Health){
			APickupHealth* TempHealth=Cast<APickupHealth>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempHealth);

			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx3=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;
				
			}
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Boost){
			APickupBoost* TempBoost=Cast<APickupBoost>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempBoost);


			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx4=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;
				
			}
		}

		
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Helmet||
		ReadyPickupItem->ItemType==E_ItemType::EIT_Vest||
		ReadyPickupItem->ItemType==E_ItemType::EIT_Backpack){
			APickupEquipment* TempEquipment=Cast<APickupEquipment>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempEquipment);

			bool bIsSucceed=PickupEquipment(TempPickupBase);
			bool bIsSuccessx3=Cast<AActor>(ReadyPickupItem)->Destroy();
			if(!bIsSucceed){
				return false;
			}
			return true;
		}
		
		
		
		
	}
	else{
		return false;
	}
	
	return false;
}

void ASPlayerController::PickupWeapon(class APickupWeapon* PickupWeaponx1,bool bIsAssign,E_WeaponPosition Positionx1){
	E_WeaponPosition TargetPosition;
	bool TargetbIsOnHand;
	AItemWeapon* ReplaceWeapon=nullptr;
	if(bIsAssign){
		AssignPosition(Positionx1,TargetPosition,TargetbIsOnHand);
	}
	else{
		AutoPosition(TargetPosition,TargetbIsOnHand);
	}
	
	
	
	if(TargetbIsOnHand){
		if(PlayerStateRef->GetHoldGun()){
			ReplaceWeapon=PlayerStateRef->GetHoldGun();
		}
	}
	else{
		if(TargetPosition==E_WeaponPosition::EWP_Left){
			if(PlayerStateRef->GetWeapon1()){
				ReplaceWeapon=PlayerStateRef->GetWeapon1();
			}

		}
		else if(TargetPosition==E_WeaponPosition::EWP_Right){
			if(PlayerStateRef->GetWeapon2()){
				ReplaceWeapon=PlayerStateRef->GetWeapon2();
			}
			
		}
	}
	//NextStep
	if(ReplaceWeapon!=nullptr){
		
		DiscardWeapon(ReplaceWeapon);
	}
	AItemWeapon* TempWeapon;
	FTransform TempTransform;
	TempTransform.SetLocation(FVector::ZeroVector);
	TempTransform.SetRotation(FQuat(0,0,0,0));
	TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
	TempWeapon=GetWorld()->SpawnActorDeferred<AItemWeapon>(AItemWeapon::StaticClass(),TempTransform);
	if(TempWeapon){
		
		TempWeapon->Ammo=PickupWeaponx1->Ammo;
		TempWeapon->Position=TargetPosition;
		TempWeapon->ID=PickupWeaponx1->ID;
		TempWeapon->bIsOnHand=TargetbIsOnHand;
		TempWeapon->SN=PickupWeaponx1->SN;
		TempWeapon->Amount=1;	
		
		
		UGameplayStatics::FinishSpawningActor(TempWeapon,TempTransform);
	}
	if(TargetbIsOnHand){
		PlayerStateRef->SetHoldGun(TempWeapon);
	}
	else{
		if(TargetPosition==E_WeaponPosition::EWP_Left){
			PlayerStateRef->SetWeapon1(TempWeapon);
		}
		else if(TargetPosition==E_WeaponPosition::EWP_Right){
			PlayerStateRef->SetWeapon2(TempWeapon);
		}
	}
	

}

void ASPlayerController::UpdateCharacterGunState(){

	if(PlayerStateRef->GetHoldGun()){
		MyCharacterRef->SetIsHoldWeapon(1);
	}
	else{
		MyCharacterRef->SetIsHoldWeapon(0);
	}

}

void ASPlayerController::TakeBackGunMontage(){
	if(PlayerStateRef->GetHoldGun()){
		MyCharacterRef->SetIsAiming(0);
		ReverseHoldAiming();
	}
	MyCharacterRef->PlayMontage(E_MontageType::EMT_UnEquip);

}

void ASPlayerController::TakeBackWeapon(){

	MyCharacterRef->SetIsHoldWeapon(0);
	PlayerStateRef->GetHoldGun()->bIsOnHand=false;
	if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Left){
		PlayerStateRef->SetWeapon1(PlayerStateRef->GetHoldGun());
		PlayerStateRef->SetHoldGun(nullptr);
	}
	else if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Right){
		PlayerStateRef->SetWeapon2(PlayerStateRef->GetHoldGun());
		PlayerStateRef->SetHoldGun(nullptr);
	}

	if(ReadyEquipWeapon){
		MyCharacterRef->PlayMontage(E_MontageType::EMT_Equip);
	}

}

void ASPlayerController::Keyboard1KeyPressed(){
	ReadyEquipWeapon =PlayerStateRef->GetWeapon1();
	if(ReadyEquipWeapon){
		MyCharacterRef->SetIsAiming(0);			
		ReverseHoldAiming();
		if(PlayerStateRef->GetHoldGun()){
			//ReleaseFire();
			MyCharacterRef->PlayMontage(E_MontageType::EMT_UnEquip);
		}
		else{
			//ReleaseFire();
			MyCharacterRef->PlayMontage(E_MontageType::EMT_Equip);
		}
	}
}

void ASPlayerController::Keyboard2KeyPressed(){
	ReadyEquipWeapon =PlayerStateRef->GetWeapon2();
	if(ReadyEquipWeapon){
		MyCharacterRef->SetIsAiming(0);	
		ReverseHoldAiming();
		if(PlayerStateRef->GetHoldGun()){
			MyCharacterRef->PlayMontage(E_MontageType::EMT_UnEquip);
		}
		else{
			MyCharacterRef->PlayMontage(E_MontageType::EMT_Equip);
		}
	}
}


void ASPlayerController::EquipWeapon(){

	MyCharacterRef->SetIsHoldWeapon(1);

	PlayerStateRef->SetHoldGun(ReadyEquipWeapon);
	ReadyEquipWeapon->bIsOnHand=true;
	if(ReadyEquipWeapon->Position==E_WeaponPosition::EWP_Left){
		PlayerStateRef->SetWeapon1(nullptr);
		ReadyEquipWeapon=nullptr;
	}
	else if(ReadyEquipWeapon->Position==E_WeaponPosition::EWP_Right){
		PlayerStateRef->SetWeapon2(nullptr);
		ReadyEquipWeapon=nullptr;
	}


}


bool ASPlayerController::PickupGoods(APickupBase* PickupBasex1){
	if(PlayerStateRef->CheckBackpackCapacity(PickupBasex1->GetWeight())){
		
		if(PickupBasex1->ItemType==E_ItemType::EIT_Accessories){
			AItemWeaponAcc* TempWeaponAcc;
				FTransform TempTransform;
				TempTransform.SetLocation(FVector::ZeroVector);
				TempTransform.SetRotation(FQuat(0,0,0,0));
				TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
				TempWeaponAcc=GetWorld()->SpawnActorDeferred<AItemWeaponAcc>(AItemWeaponAcc::StaticClass(),TempTransform);
				if(TempWeaponAcc){
					
					
					TempWeaponAcc->ID=PickupBasex1->ID;
					
					TempWeaponAcc->SN=PickupBasex1->SN;
					TempWeaponAcc->Amount=1;	
					
					
					UGameplayStatics::FinishSpawningActor(TempWeaponAcc,TempTransform);
				}
				AItemBase* TempItemBase=Cast<AItemBase>(TempWeaponAcc);
				PlayerStateRef->AddItemsInBackpack(TempItemBase);
		}
		else if(PickupBasex1->ItemType==E_ItemType::EIT_Ammo){
			PlayerStateRef->UpdateAmmoAmount(PickupBasex1->ID,true,PickupBasex1->Amount);

		}
		else if(PickupBasex1->ItemType==E_ItemType::EIT_Health){
			AItemHealth* TempHealth;
			FTransform TempTransform;
			TempTransform.SetLocation(FVector::ZeroVector);
			TempTransform.SetRotation(FQuat(0,0,0,0));
			TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
			TempHealth=GetWorld()->SpawnActorDeferred<AItemHealth>(AItemHealth::StaticClass(),TempTransform);
			if(TempHealth){
				
				
				TempHealth->ID=PickupBasex1->ID;
				
				TempHealth->SN=PickupBasex1->SN;
				TempHealth->Amount=PickupBasex1->Amount;	
				
				
				UGameplayStatics::FinishSpawningActor(TempHealth,TempTransform);
			}
			AItemBase* TempItemBase=Cast<AItemBase>(TempHealth);
			PlayerStateRef->AddItemsInBackpack(TempItemBase);
		}
		else if(PickupBasex1->ItemType==E_ItemType::EIT_Boost){
			AItemBoost* TempBoost;
			FTransform TempTransform;
			TempTransform.SetLocation(FVector::ZeroVector);
			TempTransform.SetRotation(FQuat(0,0,0,0));
			TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
			TempBoost=GetWorld()->SpawnActorDeferred<AItemBoost>(AItemBoost::StaticClass(),TempTransform);
			if(TempBoost){
				
				
				TempBoost->ID=PickupBasex1->ID;
				
				TempBoost->SN=PickupBasex1->SN;
				TempBoost->Amount=PickupBasex1->Amount;	
				
				
				UGameplayStatics::FinishSpawningActor(TempBoost,TempTransform);
			}
			AItemBase* TempItemBasex1=Cast<AItemBase>(TempBoost);
			PlayerStateRef->AddItemsInBackpack(TempItemBasex1);
		}
		
		return true;

	}
	else{
		UE_LOG(LogTemp,Warning,TEXT("NotEnoughCapacity"));
		return false;
	}
	return false;
}

void ASPlayerController::DiscardItem(AItemBase* Itemx1){
	APickupBase* PickupBasex1;
	SpawnPickupItem(Itemx1,PickupBasex1);

	if(Itemx1->ItemType==E_ItemType::EIT_Ammo){	
		PlayerStateRef->UpdateAmmoAmount(Itemx1->ID,false,0);
		
		

	}
	if(Itemx1){
		PlayerStateRef->RemoveItemsInBackpack(Itemx1);
		bool bIsDestroyed=Cast<AActor>(Itemx1)->Destroy();
	
	}
}

bool ASPlayerController::DiscardEquipment(AItemBase* Itemx1,bool bIsCheck){
	if(bIsCheck){

	
		if(Itemx1->ItemType==E_ItemType::EIT_Backpack){
			if(PlayerStateRef->CheckReplaceBackpack(Itemx1)){

			}
			else{
				UE_LOG(LogTemp,Warning,TEXT("Cannot DiscardBackpack"));
				return false;
			}
			

		}
	}
	APickupBase* TempPickupBase;
	SpawnPickupItem(Itemx1,TempPickupBase);
	PlayerStateRef->RemoveEquipment(Itemx1);

	bool bIsDestroyed=Cast<AActor>(Itemx1)->Destroy();
	return true;
}




bool ASPlayerController::PickupEquipment(APickupBase* PickupBasex1){

	if(PickupBasex1->ItemType==E_ItemType::EIT_Backpack){
		if(PlayerStateRef->CheckReplaceBackpack(PickupBasex1)){

		}
		else{
			UE_LOG(LogTemp,Warning,TEXT("Cannot PickupBackpack"));

		}
	}

	//NextSteps
	AItemBase* CurrentItem=nullptr;
	TArray<AItemBase*> OwnEquipments=PlayerStateRef->GetEquipments();
	for(int32 i=0;i<OwnEquipments.Num();i++){
		if(OwnEquipments[i]->ItemType==PickupBasex1->ItemType){
			CurrentItem=OwnEquipments[i];
		}
	}


	if(CurrentItem){
		bool bIsSucceed= DiscardEquipment(CurrentItem,false);
	}

	AItemEquipment* TempEquipment;
	FTransform TempTransform;
	TempTransform.SetLocation(FVector::ZeroVector);
	TempTransform.SetRotation(FQuat(0,0,0,0));
	TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
	TempEquipment=GetWorld()->SpawnActorDeferred<AItemEquipment>(AItemEquipment::StaticClass(),TempTransform);
	if(TempEquipment){
		
		
		TempEquipment->ID=PickupBasex1->ID;
		
		TempEquipment->SN=PickupBasex1->SN;
		TempEquipment->Amount=PickupBasex1->Amount;	
		
		
		UGameplayStatics::FinishSpawningActor(TempEquipment,TempTransform);
	}
	PlayerStateRef->AddEquipment(TempEquipment);
	return true;

}

bool ASPlayerController::EquipAccessories(AItemBase* ItemBasex1,bool bIsFromGround,AItemWeapon* Weaponx1){
	AItemWeaponAcc* TempIWeaponAcc=nullptr;
	APickupWeaponAcc* TempPWeaponAcc=nullptr;
	AItemWeaponAcc* ReplacedAccObj=nullptr;
	E_WeaponAccType TempAccType;
	TArray<FName> TempWeaponIDs;
	AItemBase* WeaponAccObjx1;
	bool bWeaponIdFound;
	if(bIsFromGround){	
		TempPWeaponAcc=Cast<APickupWeaponAcc>(ItemBasex1);
		if(TempPWeaponAcc){
			TempAccType=TempPWeaponAcc->AccType;
			TempWeaponIDs=TempPWeaponAcc->ItemWeaponAccRow->WeaponIDs;
		}
		
	}
	else{
		TempIWeaponAcc=Cast<AItemWeaponAcc>(ItemBasex1);
		if(TempIWeaponAcc){
			TempAccType=TempIWeaponAcc->AccType;
			TempWeaponIDs=TempIWeaponAcc->ItemWeaponAccRow->WeaponIDs;
		}
	}

	if(TempWeaponIDs.Num()>0){
		for(int32 i=0;i<TempWeaponIDs.Num();i++){
			
			FName ForLoopTempText=TempWeaponIDs[i];
			FName TmpWeaponx1ID=Weaponx1->ID;
			if(ForLoopTempText!=TmpWeaponx1ID){
				return false;
			}
		}
	}

	//NextSteps

	if(TempAccType==E_WeaponAccType::EWAT_Sight){
		ReplacedAccObj=Weaponx1->SightAccActorx1;
	}
	else if(TempAccType==E_WeaponAccType::EWAT_Muzzle){
		ReplacedAccObj=Weaponx1->MuzzleAccActorx1;
	}
	else if(TempAccType==E_WeaponAccType::EWAT_Foregrip){
		ReplacedAccObj=Weaponx1->ForegripAccActorx1;
	}
	else if(TempAccType==E_WeaponAccType::EWAT_Mag){
		ReplacedAccObj=Weaponx1->MagAccActorx1;
	}
	else if(TempAccType==E_WeaponAccType::EWAT_Buttstock){
		ReplacedAccObj=Weaponx1->ButtstockAccActorx1;
	}
	
	//NextStep
	if(ReplacedAccObj){
		 
		if(bIsFromGround){
			bool bIsEnoughx1=PlayerStateRef->CheckBackpackCapacity(ReplacedAccObj->GetWeight());
		
			if(bIsEnoughx1){
				AItemBase* TempIBase=Cast<AItemBase>(ReplacedAccObj);
				if(TempIBase){
					PlayerStateRef->AddItemsInBackpack(TempIBase);
				}
				
			}
			else{
				UE_LOG(LogTemp,Warning,TEXT("NotEnoughtCapacity"));
				return false;
			} 
		}
		else{
			int32 WeightDelta=ReplacedAccObj->GetWeight()-ItemBasex1->GetWeight();
		
			bool bIsEnoughx1=PlayerStateRef->CheckBackpackCapacity(WeightDelta);
			if(bIsEnoughx1){
				AItemBase* TempIBase=Cast<AItemBase>(ReplacedAccObj);
				if(TempIBase){
					PlayerStateRef->AddItemsInBackpack(TempIBase);
				}
				
			}
			else{
				UE_LOG(LogTemp,Warning,TEXT("NotEnoughtCapacity"));
				return false;
			} 
		}
	}

	//NextStep

	if(bIsFromGround){
		AItemWeaponAcc* TempWeaponAccObjx1;
		FTransform TempTransform;
		TempTransform.SetLocation(FVector::ZeroVector);
		TempTransform.SetRotation(FQuat(0,0,0,0));
		TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
		TempWeaponAccObjx1=GetWorld()->SpawnActorDeferred<AItemWeaponAcc>(AItemWeaponAcc::StaticClass(),TempTransform);
		if(TempWeaponAccObjx1){
			
			
			TempWeaponAccObjx1->ID=ItemBasex1->ID;
			
			TempWeaponAccObjx1->SN=ItemBasex1->SN;
			TempWeaponAccObjx1->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempWeaponAccObjx1,TempTransform);
		}
		WeaponAccObjx1=Cast<AItemBase>(TempWeaponAccObjx1);
		bool bIsDestroyed=Cast<AActor>(ItemBasex1)->Destroy();
	}
	else{
		PlayerStateRef->RemoveItemsInBackpack(ItemBasex1);
		WeaponAccObjx1=ItemBasex1;
	}

	//NextSteps
	AItemWeaponAcc* TmpWeaponAcc=Cast<AItemWeaponAcc>(WeaponAccObjx1);
	if(TmpWeaponAcc){
		PlayerStateRef->UpdateWeaponAcc(Weaponx1->Position,TmpWeaponAcc->AccType,TmpWeaponAcc);
	}
	
	return false;
}

bool ASPlayerController::RemoveAccessories(AItemBase* ItemAccx1,bool bIsToGround,AItemWeapon* Weaponx1){
	APickupBase* TempPickupBase=nullptr;
	AItemWeaponAcc* TempItemAcc=nullptr;
	if(bIsToGround){	
		SpawnPickupItem(ItemAccx1,TempPickupBase);
		TempItemAcc=Cast<AItemWeaponAcc>(ItemAccx1);
		if(TempItemAcc){
			PlayerStateRef->UpdateWeaponAcc(Weaponx1->Position,TempItemAcc->AccType,nullptr);
		}
		bool bIsDestroyed=Cast<AActor>(ItemAccx1)->Destroy(); 		
	}
	else{
		bool TempBackpackCapacity=PlayerStateRef->CheckBackpackCapacity(ItemAccx1->GetWeight());
		TempItemAcc=Cast<AItemWeaponAcc>(ItemAccx1);
		if(TempBackpackCapacity){
			if(TempItemAcc){
				PlayerStateRef->UpdateWeaponAcc(Weaponx1->Position,TempItemAcc->AccType,nullptr);
				PlayerStateRef->AddItemsInBackpack(ItemAccx1);
				return true;
			}
			
		}
		else{
			UE_LOG(LogTemp,Warning,TEXT("BackpackFull"));
			return false;
		}
	}

	return false;
}





















//ASPlayerController
