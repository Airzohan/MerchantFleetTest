// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/Delegate.h"
#include "MF_GameState.generated.h"

class AMF_ZoneBase;
class AMF_ShipBase;
class AMF_PortBase;
class UUserWidget;



UCLASS(Blueprintable)
class MERCHANTFLEET_API AMF_GameState : public AGameStateBase
{
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangePrice);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewDay);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeMoney, int32, Money);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddNewShip);
	
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<AMF_PortBase*> Ports;

	UPROPERTY(BlueprintReadOnly)
	TArray<AMF_ShipBase*> Ships;

	UPROPERTY(BlueprintReadOnly)
	TArray<AMF_ZoneBase*> Zones;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> PortTags;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> SpawnZoneLocations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMF_ZoneBase> ZonePiratesClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMF_ZoneBase> ZoneStormClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Money = 10000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DayForChangePrice = 7;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SecondsPerDay = 5.f;

	UPROPERTY(BlueprintReadWrite)
	bool Autotrade = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxShips = 3;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnChangeMoney OnChangeMoney;

	UPROPERTY(BlueprintAssignable)
	FOnAddNewShip OnAddNewShip;

	UPROPERTY(BlueprintAssignable)
	FOnNewDay OnNewDay;

	UPROPERTY(BlueprintAssignable)
	FOnChangePrice OnChangePrice;

	void SpawnZone();
	
	void ChangePrice();

	void NewDay();
	
	UFUNCTION()
	void AddMoney(int32 Delta);

	UFUNCTION()
	void AddNewShip(AMF_ShipBase* Ship);

	UFUNCTION(BlueprintCallable)
	bool CheckMoney(int32 Delta);

	UFUNCTION()
	bool CanBuyShip();

	UFUNCTION()
	void GetSpawnZoneLocations();

	UFUNCTION()
	void GetPorts();

	UFUNCTION()
	void CreateMainWidget();

	UFUNCTION()
	void SpawnShipInRandPort();
};
