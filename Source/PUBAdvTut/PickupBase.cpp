// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"


APickupBase::APickupBase()
{
    StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMesh->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    
    

    Outline=CreateDefaultSubobject<UStaticMeshComponent>("Outline");
    Outline->SetupAttachment(StaticMesh);
    Outline->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    UI=CreateDefaultSubobject<UWidgetComponent>("UI");

    Box=CreateDefaultSubobject<UBoxComponent>("Box");
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);

    Box->SetBoxExtent(FVector(220.f,220.f,100.f));
    

    Box->SetupAttachment(RootComponent);



}

void APickupBase::BeginPlay(){
    Super::BeginPlay();

    // Setup overlap for AreaSphere
	Box->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnBoxOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &APickupBase::OnBoxEndOverlap);

    FHitResult HitResult;
	const FVector Start{ GetActorLocation() };
	const FVector End{ Start + FVector(0.f, 0.f, -1000.f) };
	

	GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECollisionChannel::ECC_WorldStatic);
	
    if(HitResult.bBlockingHit){
        const FVector Endx{ HitResult.Location + FVector(0.f, 0.f, 10.f) };
        SetActorLocation(Endx);
    }
    
    
    


}

void APickupBase::InitPickup(E_ItemType ItemTypex1,FText Namex1,FText UI_Prefix1,UStaticMesh* StaticMeshx1 ){

    Init(ItemTypex1,Namex1);
    UI_Prefix=UI_Prefix1;
    StaticMesh->SetStaticMesh(StaticMeshx1);

}



void APickupBase::EnabledOutLine(bool bIsEnable){
    if(bIsEnable){
        StaticMesh->SetRenderCustomDepth(true);
    }
    else{
        StaticMesh->SetRenderCustomDepth(false);
    }
}


void APickupBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult){
    
    if(OtherActor){
        ASCharacter* Char=Cast<ASCharacter>(OtherActor);
        if(Char){
            OnBeginOverlap.Broadcast(this);
            
        }
        
    }
    

}

void APickupBase::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex){
    
    if(OtherActor){
        ASCharacter* Char=Cast<ASCharacter>(OtherActor);
        if(Char){
            OnEndOverlap.Broadcast(this);
            

        }
        
    }
}

