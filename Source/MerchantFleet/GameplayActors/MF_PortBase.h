// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MF_PortBase.generated.h"

struct FTradeInfo;
class AMF_ShipBase;

UCLASS()
class MERCHANTFLEET_API AMF_PortBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMF_PortBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UPortInventoryComponent* PortInventoryComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* Box;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* ParkingBox;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* Widget;

	UPROPERTY(BlueprintReadWrite)
	bool Occupied = false;

	UPROPERTY(BlueprintReadWrite)
	class AMF_ShipBase* Ship = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTradeInfo> TradeList;

	UPROPERTY(BlueprintReadWrite)
	class AMF_GameState* GS = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AMF_ShipBase> ShipClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> TradeWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BuyShipWidgetClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	class UPortTrade* TradeWidget = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void OnChangePrice();

	UFUNCTION()
	void Autotrade();

	UFUNCTION()
	void Autosell();

	UFUNCTION()
	void Autobuy();

	UFUNCTION(BlueprintCallable)
	void TryTrade(EResourceType Type, int32 ItemCount, int32 TotalCost, bool bIsBuy);

	UFUNCTION()
	void UpdateTradeList(bool Success);

	UFUNCTION()
	void OnClickedBox(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnNewDay();

	UFUNCTION()
	void SpawnShip();

	UFUNCTION()
	void UpdatePortName(FName PortName);

	UFUNCTION(BlueprintCallable)
	bool BuyShip();
};
