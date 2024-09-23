
#pragma once

#include "CoreMinimal.h"
#include "GameplayActors/MF_ZoneBase.h"
#include "MF_ZoneStorm.generated.h"

/**
 * 
 */
UCLASS()
class MERCHANTFLEET_API AMF_ZoneStorm : public AMF_ZoneBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Coeff = 0.5f;
	
	virtual void ShipOverlap(AMF_ShipBase* Ship, bool bIsBegin) override;
};
