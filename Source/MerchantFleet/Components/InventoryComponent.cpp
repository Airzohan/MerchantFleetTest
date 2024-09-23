
#include "Components/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HoldInfo.Max = 100;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateHold();
}

void UInventoryComponent::UpdateHold()
{
	int32 LoadHold = 0;
	TArray<int32> Values;
	HoldInfo.Hold.GenerateValueArray(Values);
	for (int32 i : Values)
	{
		LoadHold += i;
	}
	HoldInfo.Current = LoadHold;
	OnUpdateHold.Broadcast(HoldInfo);
}

void UInventoryComponent::RemoveItems(EResourceType Type, int32 Count)
{
	if (HoldInfo.Hold.Contains(Type))
	{
		int32 NewCount = HoldInfo.Hold.FindRef(Type) - Count;
		if (NewCount > 0)
		{
			HoldInfo.Hold.Add(Type, NewCount);
		}
		else
		{
			HoldInfo.Hold.Remove(Type);
		}
	}
	else
	{
		HoldInfo.Hold.Add(Type, Count);
	}
	UpdateHold();
}

void UInventoryComponent::AddItems(EResourceType Type, int32 Count)
{
	int32 NewCount = HoldInfo.Hold.FindRef(Type) + Count;

	if (NewCount <= HoldInfo.Max)
	{
		if (HoldInfo.Hold.Contains(Type))
		{
			HoldInfo.Hold.Add(Type, NewCount);
		}
		else
		{
			HoldInfo.Hold.Add(Type, Count);
		}
		UpdateHold();
	}
}

