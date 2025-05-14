// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Map/MapWidget.h"

#include "Components/Image.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"

UMapWidget::UMapWidget()
{
	bIsInPlane = false;
}

void UMapWidget::InitializeImage()
{
	UpdateAirplanebVisibilty();
	UpdateAirplanebLocation();
	UpdatePlayerLocation();
	UpdateCurrentZone();
	UpdateNextZone();
	UpdateRedZone();
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetLandScapeBoundingBox();
	
	if (Image_CurrentZone && Image_CurrentZone->GetDynamicMaterial())
	{
		CurrentZoneMaterial = Image_CurrentZone->GetDynamicMaterial();
	}
	if (Image_NextZone && Image_NextZone->GetDynamicMaterial())
	{
		NextZoneMaterial = Image_NextZone->GetDynamicMaterial();
	}
	if (Image_AirplaneLine && Image_AirplaneLine->GetDynamicMaterial())
	{
		AirplaneLineMaterial = Image_AirplaneLine->GetDynamicMaterial();
	}
	if (Image_RedZone && Image_RedZone->GetDynamicMaterial())
	{
		RedZoneMaterial = Image_RedZone->GetDynamicMaterial();
	}

	InitializeImage();
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdatePlayerLocation();
}

void UMapWidget::UpdatePlayerLocation()
{	
	if (bIsInPlane)
	{
		ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

		if (!GS)
		{
			return;		
		}
		
		PlayerLocation = ConvertWorldToMap(GS->CurrentAirplaneLocation);
	}
	else
	{
		if (APawn* Player = GetOwningPlayerPawn())
		{
			PlayerLocation = ConvertWorldToMap(Player->GetActorLocation());
		}
	}
	
	Image_Player->SetRenderTranslation(PlayerLocation);
}

float UMapWidget::ConvertScale(float Scale)
{
	return ((Scale) * 0.5f / LandScapeBoundingBox.Max.X);
}

void UMapWidget::UpdateCurrentZone()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	
	FVector Center = GS->CurrentZoneCenter;
	float Scale = GS->CurrentZoneScale;
		
	FVector2D NewLocation = ConvertPosition(Center);
	
	Scale = ConvertScale(Scale * 100.f);
	
	if (CurrentZoneMaterial)
	{
		CurrentZoneMaterial->SetVectorParameterValue(FName("Center"), FVector(NewLocation.X, NewLocation.Y, 0));
		CurrentZoneMaterial->SetScalarParameterValue(FName("Radius"), Scale);
	}
}

void UMapWidget::UpdateNextZone()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	
	bool IsVisibiltyNextZone = GS->bIsVisibiltyNextZone;

	if (!IsVisibiltyNextZone)
	{
		Image_NextZone->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	Image_NextZone->SetVisibility(ESlateVisibility::Visible);
	
	FVector Center = GS->NextZoneCenter;
	float Scale = GS->NextZoneScale;
	
	FVector2D NewLocation = ConvertPosition(Center);	
	Scale = ConvertScale(Scale * 100.f);
	
	if (NextZoneMaterial)
	{
		NextZoneMaterial->SetVectorParameterValue(FName("Center"), FVector(NewLocation.X, NewLocation.Y, 0));
		NextZoneMaterial->SetScalarParameterValue(FName("Radius"), Scale);
	}
}

void UMapWidget::UpdateAirplanebVisibilty()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	
	bIsInPlane = GS->bIsVisibiltyAirplane;

	if (!bIsInPlane)
	{
		Image_Airplane->SetVisibility(ESlateVisibility::Hidden);
		Image_AirplaneLine->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	FVector Direction = (GS->EndAirplanePoint - GS->StartAirplanePoint).GetSafeNormal();
	float Angle = FMath::Atan2(Direction.Y, Direction.X) * (180.0f / PI) + 90.f;

	Image_Airplane->SetRenderTransformAngle(Angle);	
	Image_Airplane->SetVisibility(ESlateVisibility::Visible);

	
	FVector2D Start = ConvertPosition(GS->StartAirplanePoint);
	FVector2D End = ConvertPosition(GS->EndAirplanePoint);

	if (AirplaneLineMaterial)
	{
		AirplaneLineMaterial->SetVectorParameterValue(FName("End"), FVector(End.X, End.Y, 0));
		AirplaneLineMaterial->SetVectorParameterValue(FName("Start"), FVector(Start.X, Start.Y, 0));
	}
	
	Image_AirplaneLine->SetVisibility(ESlateVisibility::Visible);
}

void UMapWidget::UpdateAirplanebLocation()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	
	FVector2D NewLocation = ConvertWorldToMap(GS->CurrentAirplaneLocation);
	Image_Airplane->SetRenderTranslation(NewLocation);
}

void UMapWidget::UpdateRedZone()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	
	bool bIsVisibiltyRedZone = GS->bIsVisibiltyRedZone;

	if (!bIsVisibiltyRedZone)
	{
		Image_RedZone->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	Image_RedZone->SetVisibility(ESlateVisibility::Visible);
	
	FVector Center = GS->RedZoneCenter;
	float Scale = GS->RedZoneScale;
	
	FVector2D NewLocation = ConvertPosition(Center);	
	Scale = ConvertScale(Scale);
	
	if (RedZoneMaterial)
	{
		RedZoneMaterial->SetVectorParameterValue(FName("Center"), FVector(NewLocation.X, NewLocation.Y, 0));
		RedZoneMaterial->SetScalarParameterValue(FName("Radius"), Scale);
	}
}

void UMapWidget::SetLandScapeBoundingBox() 
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;
	}

	LandScapeBoundingBox = GS->GetLandScapeBoundingBox();
}

FVector2D UMapWidget::GetPlayerLocation() const
{
	return PlayerLocation;
}

FVector2D UMapWidget::GetWorldMapImageSize() const
{
	return Image_WorldMap->GetBrush().ImageSize;
}

bool UMapWidget::GetIsInPlane() const
{
	return bIsInPlane;
}

void UMapWidget::SetIsInPlane(bool NewInIsInPlane)
{
	bIsInPlane = NewInIsInPlane;
}

FVector2D UMapWidget::ConvertPosition(const FVector& WorldLocation) const
{
	float MaxX = LandScapeBoundingBox.Max.X;
	float MaxY = LandScapeBoundingBox.Max.Y;
	float MinX = LandScapeBoundingBox.Min.X;
	float MinY = LandScapeBoundingBox.Min.Y;	

	float X = (WorldLocation.X - MinX) / (MaxX - MinX);
	float Y = (WorldLocation.Y - MinY) / (MaxY - MinY);
	
	return FVector2D(X, Y);
}

FVector2D UMapWidget::ConvertWorldToMap(const FVector& WorldLocation) const
{
	FVector2D Position = ConvertPosition(WorldLocation);
	
	float MapWidth = Image_WorldMap->GetBrush().ImageSize.X;
	float MapHeight = Image_WorldMap->GetBrush().ImageSize.Y;
	
	float UI_X = Position.X * MapWidth;
	float UI_Y = Position.Y * MapHeight;
	
	return FVector2D(UI_X, UI_Y);
}
