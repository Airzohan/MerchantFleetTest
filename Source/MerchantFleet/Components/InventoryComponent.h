
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Definitions/MF_Definitions.h"
#include "Delegates/Delegate.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTFLEET_API UInventoryComponent : public UActorComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHold, FLoadInfo, HoldInfo);
	
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:

	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLoadInfo HoldInfo;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateHold OnUpdateHold;

	UFUNCTION()
	void UpdateHold();

	UFUNCTION()
	virtual void RemoveItems(EResourceType Type, int32 Count);

	UFUNCTION()
	virtual void AddItems(EResourceType Type, int32 Count);
		
};
