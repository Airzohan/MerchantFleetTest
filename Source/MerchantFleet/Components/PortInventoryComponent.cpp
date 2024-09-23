
#include "PortInventoryComponent.h"
#include "DataAssets/PDA_Prices.h"

void UPortInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Prices != nullptr)
	{
		BasePrices = Prices->BasePrices;
		BaseItems = Prices->BaseItems;
	}
	UpdateResources();
}

void UPortInventoryComponent::AddItems(EResourceType Type, int32 Count)
{
	Super::AddItems(Type, Count);

	MaxSells.Add(Type, MaxSells.FindRef(Type) - Count);
}

void UPortInventoryComponent::ChangePrices()
{
	FPricesInfo Price;
	TArray<EResourceType> Keys;
	BasePrices.GetKeys(Keys);
	PriceCoeff = FMath::Clamp(FMath::FRandRange(0.6f, 3.0f), 0.7f, 1.3f);
	for (EResourceType Key : Keys)
	{
		if (BasePrices.Contains(Key))
		{
			ResourcePrices.Add(Key, PriceCoeff * BasePrices.FindRef(Key));
		}
	}
	Price.Prices = ResourcePrices;
	OnUpdatePrices.Broadcast(Price);
}

void UPortInventoryComponent::UpdateResources()
{
	TArray<EResourceType> Keys;
	BaseItems.GetKeys(Keys);
	for (EResourceType Key : Keys)
	{
		FResourceInfo Info = BaseItems.FindRef(Key);
		int32 NewSellCount = FMath::TruncToInt32(FMath::FRandRange(0.7f, 1.3f) * Info.SellCount);
		int32 NewBuyCount = FMath::TruncToInt32(FMath::FRandRange(0.7f, 1.3f) * Info.BuyCount);
		MaxSells.Add(Key,NewSellCount);
		HoldInfo.Hold.Add(Key, NewBuyCount);
	}
}
