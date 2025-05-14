// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/Compass/CompassWidget.h"

#include "Character/PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCompassWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Directional = Image_Compass->GetDynamicMaterial();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	//PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	
}

void UCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	
	FRotator ControlRotation = PlayerController->GetControlRotation();
	float YawValue = ControlRotation.Yaw;
	
	
	Directional->SetScalarParameterValue(FName("U"), YawValue / 360.0f);
	Calculate(YawValue);
	
	
}

void UCompassWidget::Calculate(float Value)
{
	if (Value < 0.0f)
	{
		SetDirectionValue(Value + 360.0f);
	}
	else
	{
		SetDirectionValue(Value);
	}
}

void UCompassWidget::SetDirectionValue(float Value)
{
	int32 OutValue = FMath::RoundToInt(Value / 5.0f) * 5;
	SetDirectionText(OutValue);
}

void UCompassWidget::SetDirectionText(int32 Value)
{
	FText OutText;
	if (Value % 45 == 0)
	{
		switch (Value / 45)
		{
		case 0:
			OutText = FText::FromString("N");
			break;
		case 1:
			OutText = FText::FromString("NE");
			break;
		case 2:
			OutText = FText::FromString("E");
			break;
		case 3:
			OutText = FText::FromString("SE");
			break;
		case 4:
			OutText = FText::FromString("S");
			break;
		case 5:
			OutText = FText::FromString("SW");
			break;
		case 6:
			OutText = FText::FromString("W");
			break;
		case 7:
			OutText = FText::FromString("NW");
			break;
		case 8:
			OutText = FText::FromString("N");
			break;
		}
	}
	else
	{
		OutText = FText::AsNumber(Value);
	}

	TextBlock_Direction->SetText(OutText);

	if (OutText.EqualTo(FText::FromString("N")))
	{
		TextBlock_Direction->SetColorAndOpacity(FSlateColor(FLinearColor(0.982251f, 0.701102f, 0.0f, 1.0f)));
	}
	else
	{
		TextBlock_Direction->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
	}
}

