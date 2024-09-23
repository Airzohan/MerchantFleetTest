
#pragma once

#include "CoreMinimal.h"
#include "GameplayActors/MF_ZoneBase.h"
#include "MF_ZonePirates.generated.h"

/**
 * 
 */
UCLASS()
class MERCHANTFLEET_API AMF_ZonePirates : public AMF_ZoneBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float CoeffMax = 0.3f;
	UPROPERTY(EditAnywhere)
	float CoeffMin = 0.7f;
	
	virtual void ShipOverlap(AMF_ShipBase* Ship, bool bIsBegin) override;
};
