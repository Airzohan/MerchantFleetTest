
#include "GameplayActors/MF_ZoneStorm.h"

void AMF_ZoneStorm::ShipOverlap(AMF_ShipBase* Ship, bool bIsBegin)
{
	Super::ShipOverlap(Ship, bIsBegin);
	Ship->ChangeSpeed(Coeff,bIsBegin);
}
