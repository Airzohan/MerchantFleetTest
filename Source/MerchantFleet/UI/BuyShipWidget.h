// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuyShipWidget.generated.h"

/**
 * 
 */
UCLASS()
class MERCHANTFLEET_API UBuyShipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMF_PortBase* Port = nullptr;
};
