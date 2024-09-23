// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortName.generated.h"

/**
 * 
 */
UCLASS()
class MERCHANTFLEET_API UPortName : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateName(FName PortName);
};
