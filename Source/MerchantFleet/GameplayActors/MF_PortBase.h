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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPortInventoryComponent* PortInventoryComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* ParkingBox;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Occupied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMF_ShipBase* Ship = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTradeInfo> TradeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMF_GameState* GS = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMF_ShipBase> ShipClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> TradeWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BuyShipWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
