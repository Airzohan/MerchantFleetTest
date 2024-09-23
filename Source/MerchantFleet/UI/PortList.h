// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Definitions/MF_Definitions.h"
#include "PortList.generated.h"

/**
 * 
 */
UCLASS()
class MERCHANTFLEET_API UPortList : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FPortInfo> PortList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMF_ShipBase* Ship = nullptr;

	UFUNCTION(BlueprintNativeEvent)
	void UpdateWidget();
};
