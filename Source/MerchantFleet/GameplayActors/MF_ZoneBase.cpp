
#include "GameplayActors/MF_ZoneBase.h"

#include "MF_ShipBase.h"
#include "Components/SphereComponent.h"


AMF_ZoneBase::AMF_ZoneBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AMF_ZoneBase::BeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AMF_ZoneBase::EndOverlap);
	Sphere->SetHiddenInGame(false);
}

void AMF_ZoneBase::ShipOverlap(AMF_ShipBase* Ship, bool bIsBegin)
{
}

void AMF_ZoneBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMF_ShipBase* Ship = Cast<AMF_ShipBase>(OtherActor))
	{
		ShipOverlap(Ship,true);
	}
}

void AMF_ZoneBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMF_ShipBase* Ship = Cast<AMF_ShipBase>(OtherActor))
	{
		ShipOverlap(Ship,false);
	}
}

