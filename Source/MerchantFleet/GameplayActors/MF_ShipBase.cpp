
#include "EngineUtils.h"
#include "GameplayActors/MF_ShipBase.h"
#include "Components/BoxComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/PortInventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "GameplayActors/MF_PortBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/MF_GameState.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "UI/PortList.h"
#include "DataAssets/PDA_Prices.h"
#include "AIController.h"
#include "UI/ShipWidget.h"

AMF_ShipBase::AMF_ShipBase()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(StaticMesh);
	Box->OnClicked.AddDynamic(this, &AMF_ShipBase::OnClickedBox);
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
}

void AMF_ShipBase::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkSpeedBase = GetCharacterMovement()->MaxWalkSpeed;

	ShipController = Cast<AAIController>(GetController());
	if (ShipController)
	{
		ShipController->ReceiveMoveCompleted.AddDynamic(this,&AMF_ShipBase::MoveCompleted);
	}
	if (DA_Prices)
	{
		LastBuyPrices = DA_Prices->BasePrices;
	}
	if (UShipWidget* ShipWidget = Cast<UShipWidget>(Widget->GetWidget()))
	{
		ShipWidget->OnClickedAuto.AddDynamic(this,&AMF_ShipBase::AutoSelect);
		ShipWidget->OnClickedSelectPort.AddDynamic(this,&AMF_ShipBase::OnClickedSelectPort);
	}
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&AMF_ShipBase::AlternativeBeginPlay,0.3f,false);
}

void AMF_ShipBase::AlternativeBeginPlay()
{
	ActivateAutotrade(Autotrade);
}

void AMF_ShipBase::OnClickedBox(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (GetCharacterMovement()->Velocity.Size() == 0.f)
	{
		ChangeVisibilityWidget(true,ESlateVisibility::Visible);
	}
}

void AMF_ShipBase::ChangeVisibilityWidget(bool Flip, ESlateVisibility Visibility)
{
	if (Widget->GetWidget() == nullptr)
	{
		return;
	}
	if (Flip)
	{
		if (Visibility == ESlateVisibility::Hidden)
		{
			Widget->GetWidget()->SetVisibility(ESlateVisibility::Visible);
		}
		else if (Visibility == ESlateVisibility::Visible)
		{
			Widget->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	Widget->GetWidget()->SetVisibility(Visibility);
}

void AMF_ShipBase::OnClickedAutoSelect()
{
	if (CurrentPort)
	{
		CurrentPort->Autotrade();
	}
}

void AMF_ShipBase::OnClickedSelectPort()
{
	if (PortListWidgetClass == nullptr){return;}
	
	if (GeneratePortList(OutList))
	{
		PortListWidget = CreateWidget<UPortList>(GetWorld(),PortListWidgetClass);
		if (PortListWidget)
		{
			PortListWidget->PortList = OutList;
			PortListWidget->Ship = this;
			PortListWidget->UpdateWidget();
			PortListWidget->AddToViewport(0);
		}
	}
}

void AMF_ShipBase::AutoSelect()
{
	bool Success = GeneratePortList(OutList);
	if (Success)
	{
		FName PortName = "";
		GetBestPort(PortName);
		ToNewPort(PortName);
	}
}

void AMF_ShipBase::ToNewPort(FName PortTag)
{
	for (TActorIterator<AMF_PortBase> It(GetWorld()); It; ++It)
	{
		AMF_PortBase* Port = *It;
		if (Port)
		{
			if (Port->Tags.Contains(PortTag))
			{
				TargetPort = Port;
				break;
			}
		}
	}
	if (TargetPort)
	{
		if (ShipController)
		{
			ChangeVisibilityWidget(false, ESlateVisibility::Hidden);
			ShipController->MoveToLocation(TargetPort->ParkingBox->GetComponentLocation(), 10.f);
		}
		if (PortListWidget)
		{
			PortListWidget->RemoveFromParent();
			PortListWidget = nullptr;
		}
		if (CurrentPort)
		{
			CurrentPort->Ship = nullptr;
			CurrentPort->Occupied = false;
			TargetPort->Occupied = true;
		}
	}
}

bool AMF_ShipBase::GeneratePortList(TMap<FName, FPortInfo>& List)
{
	AMF_GameState* GS = Cast<AMF_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	TMap<FName, FPortInfo> TempList;
	if (GS)
	{
		for (AMF_PortBase* Port : GS->Ports)
		{
			if (Port && !Port->Occupied && Port->Tags.IsValidIndex(0) && NavSystem && Port != CurrentPort)
			{
				float PathLength = 0.f;
				const FVector Start = GetActorLocation();
				const FVector End = Port->ParkingBox->GetComponentLocation();
				NavSystem->GetPathLength(GetActorLocation(), Port->ParkingBox->GetComponentLocation(), PathLength);
				FPortInfo Info;
				Info.PriceCoeff = Port->PortInventoryComp->PriceCoeff;
				Info.DayToPort = FMath::TruncToInt32((PathLength/MaxWalkSpeedBase)/GS->SecondsPerDay);
				TempList.Add(Port->Tags[0], Info);
			}
		}
	}
	List = TempList;
	return !List.IsEmpty();
}

void AMF_ShipBase::GetBestPort(FName& PortName)
{
	PortName = "none";
	TArray<float> Points;
	TArray<FName> Keys;
	TArray<FPortInfo> Arr;
	OutList.GenerateValueArray(Arr);
	for (FPortInfo Info : Arr)
	{
		Points.Add(GetFloat(Info));
	}
	OutList.GenerateKeyArray(Keys);
	int32 MaxIndex = 0;
	float MaxFloat = 0.f;
	UKismetMathLibrary::MaxOfFloatArray(Points, MaxIndex, MaxFloat);
	if (Keys.IsValidIndex(MaxIndex))
	{
		PortName = Keys[MaxIndex];
	}
}

float AMF_ShipBase::GetFloat(FPortInfo PortInfo)
{
	float Point = 0.f;
	float LoadHold = InventoryComp->HoldInfo.Current / InventoryComp->HoldInfo.Max;
	if (LoadHold > ThresholdMaxLoadHoldToSell || CurrentPort->GS->Money < ThresholdMinMoneyToBuy)
	{
		Point = FMath::GetMappedRangeValueClamped(FVector2D(0.7f, 1.3f), FVector2D(0.f, 1.f), PortInfo.PriceCoeff);
	}
	else if (LoadHold < ThresholdMinLoadHoldToBuy)
	{
		Point = FMath::GetMappedRangeValueClamped(FVector2D(0.7f, 1.3f), FVector2D(1.f, 0.f), PortInfo.PriceCoeff);
	}
	else
	{
		float PricePoint = FMath::GetMappedRangeValueClamped(FVector2D(0.7f, 1.3f), FVector2D(0.f, 1.f), PortInfo.PriceCoeff);
		float DayPoint = FMath::GetMappedRangeValueClamped(FVector2D(0.7f, 1.3f), FVector2D(0.f, 1.f), PortInfo.DayToPort);
		Point = (PricePoint + DayPoint) * 0.5f;
	}
	return Point;
}

void AMF_ShipBase::ActivateAutotrade(bool Activate)
{
	Autotrade = Activate;
	if (Activate)
	{
		if (CurrentPort)
		{
			CurrentPort->Autotrade();
		}
	}
}

void AMF_ShipBase::ChangeSpeed(float Coeff, bool Activate)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Activate ? GetCharacterMovement()->MaxWalkSpeed * Coeff : MaxWalkSpeedBase;
	}
}

void AMF_ShipBase::PirateAttack(float Coeff)
{
	TArray<EResourceType> Keys;
	InventoryComp->HoldInfo.Hold.GetKeys(Keys);
	for (EResourceType Key : Keys)
	{
		int32 NewValue = FMath::TruncToInt32(InventoryComp->HoldInfo.Hold.FindRef(Key) * Coeff);
		InventoryComp->HoldInfo.Hold.Add(Key, NewValue);
	}
	InventoryComp->UpdateHold();
}

void AMF_ShipBase::MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Type::Success)
	{
		SetActorRotation(TargetPort->ParkingBox->GetComponentRotation());
		CurrentPort = TargetPort;
		CurrentPort->Ship = this;
		TargetPort = nullptr;
		CurrentPort->Occupied = true;
		if (Autotrade)
		{
			CurrentPort->Autotrade();
		}
	}
}
