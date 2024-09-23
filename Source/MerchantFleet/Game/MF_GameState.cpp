
#include "Game/MF_GameState.h"
#include "GameplayActors/MF_PortBase.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/PortInventoryComponent.h"

void AMF_GameState::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle ChangePriceTimer;
	FTimerHandle NewDayTimer;
	GetWorldTimerManager().SetTimer(ChangePriceTimer,this, &ThisClass::ChangePrice,DayForChangePrice * SecondsPerDay,true);
	GetWorldTimerManager().SetTimer(NewDayTimer,this, &ThisClass::NewDay, SecondsPerDay,true);
	for (TActorIterator<AMF_PortBase> It(GetWorld()); It; ++It)
	{
		AMF_PortBase* Port = *It;
		if (Port)
		{
			Ports.Add(Port);
		}
	}
	if (MainWidgetClass)
	{
		if (UUserWidget* MainWidget = CreateWidget(GetWorld(),MainWidgetClass))
		{
			MainWidget->AddToViewport(0);
		}	
	}
	if (!Ports.IsEmpty())
	{
		AMF_PortBase* RndPort = Ports[FMath::RandRange(0,Ports.Num()-1)];
		if (RndPort)
		{
			RndPort->SpawnShip();
		}
		for (AMF_PortBase* Port : Ports)
		{
			bool Success = false;
			Port->PortInventoryComp->ChangePrices();
			Port->UpdateTradeList(Success);
			if (PortTags.IsValidIndex(Ports.IndexOfByKey(Port)))
			{
				Port->UpdatePortName(PortTags[Ports.IndexOfByKey(Port)]);
			}
		}
	}
	OnChangeMoney.Broadcast(Money);
}

void AMF_GameState::SpawnZone()
{
	
}

void AMF_GameState::ChangePrice()
{
	OnChangePrice.Broadcast();
	SpawnZone();
}

void AMF_GameState::NewDay()
{
	OnNewDay.Broadcast();
}

void AMF_GameState::AddMoney(int32 Delta)
{
	if (CheckMoney(Delta))
	{
		Money += Delta;
		OnChangeMoney.Broadcast(Money);
	}
}

void AMF_GameState::AddNewShip(AMF_ShipBase* Ship)
{
	Ships.Add(Ship);
	OnAddNewShip.Broadcast();
}

bool AMF_GameState::CheckMoney(int32 Delta)
{
	return Money + Delta >= 0;
}

bool AMF_GameState::CanBuyShip()
{
	return Ships.Num() < MaxShips;
}
