// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemsGroup.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AItemsGroup::AItemsGroup()
{
 	


	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent=SceneComponent;

	StaticMesh1=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh1");
	StaticMesh1->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh1->SetRelativeLocation(FVector(0));
	StaticMesh1->SetRelativeRotation(FRotator(0));
	StaticMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	StaticMesh2=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh2");
	StaticMesh2->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh2->SetRelativeLocation(FVector(-20,170.f,0));
	StaticMesh2->SetRelativeRotation(FRotator(0,90.f,0));
	StaticMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMesh3=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh3");
	StaticMesh3->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh3->SetRelativeLocation(FVector(20,-170.f,0));
	StaticMesh3->SetRelativeRotation(FRotator(0,-90.f,0));
	StaticMesh3->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMesh4=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh4");
	StaticMesh4->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh4->SetRelativeLocation(FVector(170,0,0));
	StaticMesh4->SetRelativeRotation(FRotator(0,0,0));
	StaticMesh4->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMesh5=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh5");
	StaticMesh5->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh5->SetRelativeLocation(FVector(-170,0,0));
	StaticMesh5->SetRelativeRotation(FRotator(0,0,0));
	StaticMesh5->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

TArray<FVector> AItemsGroup::GetAllLocations(){
	TArray<FVector> MeshTransforms;
	FVector SM1Loc=StaticMesh1->GetComponentLocation();
	FVector SM2Loc=StaticMesh2->GetComponentLocation();
	FVector SM3Loc=StaticMesh3->GetComponentLocation();
	FVector SM4Loc=StaticMesh4->GetComponentLocation();
	FVector SM5Loc=StaticMesh5->GetComponentLocation();
	MeshTransforms.Add(SM1Loc);
	MeshTransforms.Add(SM2Loc);
	MeshTransforms.Add(SM3Loc);
	MeshTransforms.Add(SM4Loc);
	MeshTransforms.Add(SM5Loc);
	return MeshTransforms;
}

