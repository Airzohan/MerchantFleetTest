// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MF_Definitions.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Cane UMETA(DisplayName = "Cane"),
	Rum UMETA(DisplayName = "Rum"),
	Wood UMETA(DisplayName = "Wood"),
	Textile UMETA(DisplayName = "Textile"),
	Nut UMETA(DisplayName = "Nut"),
	Fish UMETA(DisplayName = "Fish"),
	Meat UMETA(DisplayName = "Meat"),
	Leather UMETA(DisplayName = "Leather")
};

USTRUCT(BlueprintType)
struct FLoadInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Current = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, int32> Hold;
};

USTRUCT(BlueprintType)
struct FPortInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DayToPort = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PriceCoeff = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0.f;
};

USTRUCT(BlueprintType)
struct FResourceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuyCount = 0;
};

USTRUCT(BlueprintType)
struct FTradeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ship = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Port = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSellCount = 0;
};

USTRUCT(BlueprintType)
struct FPricesInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, int32> Prices;
};

UCLASS()
class MERCHANTFLEET_API UMF_Definitions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
