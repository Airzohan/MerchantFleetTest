// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Definitions/MF_Definitions.h"
#include "PDA_Prices.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MERCHANTFLEET_API UPDA_Prices : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, int32> BasePrices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, FResourceInfo> BaseItems;
};
