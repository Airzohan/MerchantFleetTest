// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedAuto);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedSelectPort);

UCLASS()
class MERCHANTFLEET_API UShipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnClickedAuto OnClickedAuto;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnClickedSelectPort OnClickedSelectPort;
	
};
