// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DisplayMessage/DisplayMessageItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDisplayMessageItemWidget::SetMessage(FText& NewText)
{
	Message = NewText;

	if (Message.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Text_DisplayMessageItem->SetText(Message);
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UDisplayMessageItemWidget::SetKeyTexture(UTexture2D* NewTexture)
{
	if (Image_KeyImage && NewTexture)
	{
		Image_KeyImage->SetBrushFromTexture(NewTexture);
	}
	else
	{
		Image_KeyImage->SetBrushFromTexture(FKeyTexture);
	}
}

// void UDisplayMessageItemWidget::InitializeImage()
// {
// 	Image_FKeyImage = LoadObject<UTexture2D>(nullptr,TEXT("/Script/Engine.Texture2D/Game/PUBGAsset/UI/KeyboardAndMouse/Keys/Key_Char_F.Key_Char_F"));
// }

