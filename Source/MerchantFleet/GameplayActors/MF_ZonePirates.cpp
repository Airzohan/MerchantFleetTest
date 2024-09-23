
#include "GameplayActors/MF_ZonePirates.h"

void AMF_ZonePirates::ShipOverlap(AMF_ShipBase* Ship, bool bIsBegin)
{
	Super::ShipOverlap(Ship, bIsBegin);
	Ship->PirateAttack(FMath::RandRange(CoeffMin, CoeffMax));
	SetLifeSpan(1.f);
}
