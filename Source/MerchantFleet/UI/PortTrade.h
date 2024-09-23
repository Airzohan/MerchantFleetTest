// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Definitions/MF_Definitions.h"
#include "GameplayActors/MF_PortBase.h"
#include "PortTrade.generated.h"


/**
 * 
 */
UCLASS()
class MERCHANTFLEET_API UPortTrade : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMF_PortBase* Port = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTradeInfo> TradeListInfo;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateList(const TArray<FTradeInfo>& TradeList);
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateHold(FLoadInfo Hold);
};
