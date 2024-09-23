
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Definitions/MF_Definitions.h"
#include "Navigation/PathFollowingComponent.h"
#include "MF_ShipBase.generated.h"

struct FPathFollowingResult;
struct FPortInfo;
class UUserWidget;

UCLASS()
class MERCHANTFLEET_API AMF_ShipBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMF_ShipBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* Box;

	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* Widget;

	UPROPERTY(BlueprintReadOnly)
	bool Autotrade = false;

	UPROPERTY(BlueprintReadOnly)
	float MaxWalkSpeedBase = 0.f;

	UPROPERTY(BlueprintReadWrite)
	class AMF_PortBase* CurrentPort = nullptr;

	UPROPERTY(BlueprintReadWrite)
	class AMF_PortBase* TargetPort = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PortListWidgetClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	class UPortList* PortListWidget = nullptr;

	UPROPERTY()
	class AAIController* ShipController = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TMap<EResourceType, int32> LastBuyPrices;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UPDA_Prices* DA_Prices = nullptr;

	UPROPERTY()
	TMap<FName, FPortInfo> OutList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ThresholdMaxLoadHoldToSell = 0.85f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ThresholdMinLoadHoldToBuy = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ThresholdMinMoneyToBuy = 1000;

protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

public:	

	UFUNCTION()
	void AlternativeBeginPlay();
	UFUNCTION()
	void OnClickedBox(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	
	UFUNCTION()
	void ChangeVisibilityWidget(bool Flip, ESlateVisibility Visibility);

	UFUNCTION()
	void OnClickedAutoSelect();

	UFUNCTION()
	void OnClickedSelectPort();

	UFUNCTION()
	void AutoSelect();

	UFUNCTION(BlueprintCallable)
	void ToNewPort(FName PortTag);

	UFUNCTION()
	bool GeneratePortList(TMap<FName, FPortInfo>& List);
	
	UFUNCTION()
	void GetBestPort(FName& PortName);

	UFUNCTION()
	float GetFloat(FPortInfo PortInfo);

	UFUNCTION(BlueprintCallable)
	void ActivateAutotrade(bool Activate);

	UFUNCTION()
	void ChangeSpeed(float Coeff, bool Activate);

	UFUNCTION()
	void PirateAttack(float Coeff);
};
