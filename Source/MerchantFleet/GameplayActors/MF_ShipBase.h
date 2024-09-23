
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
	// Sets default values for this character's properties
	AMF_ShipBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Autotrade = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeedBase = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMF_PortBase* CurrentPort = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMF_PortBase* TargetPort = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PortListWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPortList* PortListWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAIController* ShipController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, int32> LastBuyPrices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPDA_Prices* DA_Prices = nullptr;

	UPROPERTY()
	TMap<FName, FPortInfo> OutList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThresholdMaxLoadHoldToSell = 0.85f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThresholdMinLoadHoldToBuy = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ThresholdMinMoneyToBuy = 1000;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Widget;

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
