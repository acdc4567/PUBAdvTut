// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemsGroup.generated.h"

class USceneComponent;
class UStaticMeshComponent;


UCLASS()
class PUBADVTUT_API AItemsGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItemsGroup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UStaticMeshComponent* StaticMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UStaticMeshComponent* StaticMesh2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UStaticMeshComponent* StaticMesh3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UStaticMeshComponent* StaticMesh4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UStaticMeshComponent* StaticMesh5;

	TArray<FTransform> GetAllTransforms();

protected:
	
	


public:	
	

};
