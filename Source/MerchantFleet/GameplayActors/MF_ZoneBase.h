// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MF_ShipBase.h"
#include "GameFramework/Actor.h"
#include "MF_ZoneBase.generated.h"

UCLASS()
class MERCHANTFLEET_API AMF_ZoneBase : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	AMF_ZoneBase();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USphereComponent* Sphere;
	
	UFUNCTION()
	virtual void ShipOverlap(AMF_ShipBase* Ship, bool bIsBegin);
	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
