// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "PortInventoryComponent.generated.h"

/**
 * 
 */
class UPDA_Prices;

UCLASS()
class MERCHANTFLEET_API UPortInventoryComponent : public UInventoryComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatePrices, FPricesInfo, Prices);
	
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TMap<EResourceType, int32> ResourcePrices;

	UPROPERTY(BlueprintReadOnly)
	TMap<EResourceType, int32> BasePrices;

	UPROPERTY(BlueprintReadOnly)
	TMap<EResourceType, int32> MaxSells;

	UPROPERTY(BlueprintReadOnly)
	TMap<EResourceType, FResourceInfo> BaseItems;

	UPROPERTY(BlueprintReadOnly)
	float PriceCoeff = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPDA_Prices* Prices;

	UPROPERTY(BlueprintAssignable)
	FOnUpdatePrices OnUpdatePrices;

protected:

	virtual void BeginPlay() override;

public:	
	virtual void AddItems(EResourceType Type, int32 Count) override;

	UFUNCTION()
	void ChangePrices();

	UFUNCTION()
	void UpdateResources();

};
