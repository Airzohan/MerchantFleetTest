
#include "GameplayActors/MF_PortBase.h"
#include "Components/BoxComponent.h"
#include "GameplayActors/MF_ShipBase.h"
#include "Components/WidgetComponent.h"
#include "Components/PortInventoryComponent.h"
#include "Game/MF_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BuyShipWidget.h"
#include "UI/PortName.h"
#include "UI/PortTrade.h"

AMF_PortBase::AMF_PortBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PortInventoryComp = CreateDefaultSubobject<UPortInventoryComponent>(TEXT("Inventory"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(StaticMesh);
	ParkingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ParkingBox"));
	ParkingBox->SetupAttachment(StaticMesh);
	Box->OnClicked.AddDynamic(this, &AMF_PortBase::OnClickedBox);
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMF_PortBase::BeginPlay()
{
	Super::BeginPlay();
	GS = Cast<AMF_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangePrice.AddDynamic(this,&AMF_PortBase::OnChangePrice);
		GS->OnNewDay.AddDynamic(this,&AMF_PortBase::OnNewDay);
	}
}

void AMF_PortBase::OnChangePrice()
{
	if (!Occupied)
	{
		bool Success = false;
		PortInventoryComp->ChangePrices();
		UpdateTradeList(Success);
		if (Success)
		{
			if (TradeWidget)
			{
				TradeWidget->UpdateList(TradeList);
			}
		}
	}
}

void AMF_PortBase::Autotrade()
{
	bool Success = false;
	UpdateTradeList(Success);
	Autosell();
	Autobuy();
	Ship->AutoSelect();
}

void AMF_PortBase::Autosell()
{
	if (Ship == nullptr){ return;}

	TArray<EResourceType> AutotradeResources;
	TArray<EResourceType> Keys;
	TArray<int32> Values;
	PortInventoryComp->ResourcePrices.GenerateValueArray(Values);
	Ship->LastBuyPrices.GenerateKeyArray(Keys);
	for (EResourceType Key : Keys)
	{
		if (Values.IsValidIndex(Keys.IndexOfByKey(Key)))
		{
			if (Values[Keys.IndexOfByKey(Key)] > Ship->LastBuyPrices.FindRef(Key))
			{
				AutotradeResources.Add(Key);
			}
		}
	}
	for (FTradeInfo Info : TradeList)
	{
		if (AutotradeResources.Contains(Info.Type))
		{
			int32 ItemCount = FMath::Min(Info.Ship, Info.MaxSellCount);
			if (ItemCount > 0)
			{
				Ship->LastBuyPrices.Add(Info.Type, PortInventoryComp->BasePrices.FindRef(Info.Type));
				TryTrade(Info.Type, ItemCount, ItemCount * Info.Cost, false);
			}
		}
	}
}

void AMF_PortBase::Autobuy()
{
	if (Ship == nullptr){return;}

	bool bIsMinPriceCoeff = false;
	TArray<EResourceType> AutotradeResources;
	TArray<EResourceType> Keys;
	TArray<int32> Values;
	if (Ship->InventoryComp->HoldInfo.Current / Ship->InventoryComp->HoldInfo.Max > 0.2f)
	{
		TArray<float> PortsCoeff;
		if (GS)
		{
			for ( AMF_PortBase* Port : GS->Ports)
			{
				PortsCoeff.Add(Port->PortInventoryComp->PriceCoeff);
			}
		}
		bIsMinPriceCoeff = PortInventoryComp->PriceCoeff <= FMath::Min(PortsCoeff);
	}
	
	PortInventoryComp->ResourcePrices.GenerateValueArray(Values);
	Ship->LastBuyPrices.GenerateKeyArray(Keys);
	for (EResourceType Key : Keys)
	{
		if (bIsMinPriceCoeff)
		{
			AutotradeResources.Add(Key);
		}
		else
		{
			if (Values.IsValidIndex(Keys.IndexOfByKey(Key)))
			{
				if (Values[Keys.IndexOfByKey(Key)] < Ship->LastBuyPrices.FindRef(Key))
				{
					AutotradeResources.Add(Key);
				}
			}
		}
	}
	for (FTradeInfo Info : TradeList)
	{
		if (AutotradeResources.Contains(Info.Type))
		{
			if (GS)
			{
				int32 HoldLoad = Ship->InventoryComp->HoldInfo.Max - Ship->InventoryComp->HoldInfo.Current;
				int32 ItemCount = FMath::Min3(GS->Money/Info.Cost,Info.Port,HoldLoad);
				if (ItemCount > 0)
				{
					Ship->LastBuyPrices.Add(Info.Type, Info.Cost);
					TryTrade(Info.Type, ItemCount, ItemCount * Info.Cost, true);
				}
				else
				{
					break;
				}
			}
		}
	}
}

void AMF_PortBase::TryTrade(EResourceType Type, int32 ItemCount, int32 TotalCost, bool bIsBuy)
{
	if (GS == nullptr|| Ship == nullptr){return;}
	
	GS->AddMoney(bIsBuy ? TotalCost*-1 : TotalCost);
	if (bIsBuy)
	{
		Ship->InventoryComp->AddItems(Type,ItemCount);
		PortInventoryComp->RemoveItems(Type,ItemCount);
	}
	else
	{
		Ship->InventoryComp->RemoveItems(Type,ItemCount);
		PortInventoryComp->AddItems(Type,ItemCount);
	}
	bool Success = false;
	UpdateTradeList(Success);
	
	if (TradeWidget)
	{
		TradeWidget->UpdateList(TradeList);
		TradeWidget->UpdateHold(Ship->InventoryComp->HoldInfo);
	}
}

void AMF_PortBase::UpdateTradeList(bool Success)
{
	if (Ship == nullptr)
	{
		Success = false;
		return;
	}
	TradeList.Empty();
	TArray<EResourceType> Keys;
	PortInventoryComp->ResourcePrices.GenerateKeyArray(Keys);
	for (EResourceType Key : Keys)
	{
		FTradeInfo TradeInfo;
		TradeInfo.Type = Key;
		TradeInfo.Ship = Ship->InventoryComp->HoldInfo.Hold.FindRef(Key);
		TradeInfo.Port = PortInventoryComp->HoldInfo.Hold.FindRef(Key);
		TradeInfo.Cost = PortInventoryComp->ResourcePrices.FindRef(Key);
		TradeInfo.MaxSellCount = PortInventoryComp->MaxSells.FindRef(Key);
		TradeList.Add(TradeInfo);
	}
	Success = true;
}

void AMF_PortBase::OnClickedBox(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (Occupied)
	{
		bool Success = false;
		UpdateTradeList(Success);
		if (!TradeWidget)
		{
			TradeWidget = CreateWidget<UPortTrade>(GetWorld(),TradeWidgetClass);
			if (TradeWidget && Ship)
			{
				TradeWidget->Port = this;
				TradeWidget->UpdateList(TradeList);
				TradeWidget->UpdateHold(Ship->InventoryComp->HoldInfo);
				TradeWidget->AddToViewport(0);
			}
		}
	}
	else
	{
		if (GS && GS->CanBuyShip())
		{
			if (UBuyShipWidget* BuyShipWidget = CreateWidget<UBuyShipWidget>(GetWorld(),BuyShipWidgetClass))
			{
				BuyShipWidget->Port = this;
				BuyShipWidget->AddToViewport(0);
			}
		}
	}
}

void AMF_PortBase::OnNewDay()
{
	if (!Occupied)
	{
		PortInventoryComp->UpdateResources();
	}
}

void AMF_PortBase::SpawnShip()
{
	if (GS == nullptr){return;}
	
	const FTransform Transform = FTransform(ParkingBox->GetComponentRotation(),ParkingBox->GetComponentLocation(),FVector(1.f));
	AMF_ShipBase* NewShip = GetWorld()->SpawnActorDeferred<AMF_ShipBase>(ShipClass,Transform,this,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (NewShip)
	{
		NewShip->CurrentPort = this;
		NewShip->Autotrade = GS->Autotrade;
		NewShip->FinishSpawning(Transform);
	}
	Ship = NewShip;
	Occupied = true;
	GS->AddNewShip(Ship);
}

void AMF_PortBase::UpdatePortName(FName PortName)
{
	Tags.Add(PortName);
	if (UPortName* PortNameWidget=  Cast<UPortName>(Widget->GetWidget()))
	{
		PortNameWidget->UpdateName(PortName);
	}
}

bool AMF_PortBase::BuyShip()
{
	if (!Occupied)
	{
		if (GS && GS->CanBuyShip() && GS->CheckMoney(-20000))
		{
			GS->AddMoney(-20000);
			SpawnShip();
			return true;
		}
		return false;
	}
	return false;
}

