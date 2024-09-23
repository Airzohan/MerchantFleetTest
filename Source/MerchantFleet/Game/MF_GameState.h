// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/Delegate.h"
#include "MF_GameState.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AMF_PortBase*> Ports;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AMF_ShipBase*> Ships;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> PortTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DayForChangePrice = 7;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondsPerDay = 5.f;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnChangeMoney OnChangeMoney;

	UPROPERTY(BlueprintAssignable)
	FOnAddNewShip OnAddNewShip;

	UPROPERTY(BlueprintAssignable)
	FOnNewDay OnNewDay;

	UPROPERTY(BlueprintAssignable)
	FOnChangePrice OnChangePrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Autotrade = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxShips = 3;

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
};
