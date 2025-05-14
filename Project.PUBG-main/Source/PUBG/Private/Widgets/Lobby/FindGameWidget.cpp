// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/FindGameWidget.h"

#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Widgets/Lobby/BaseButtonWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "Widgets/Lobby/ServerBrowserItemWidget.h"

void UFindGameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button_Refresh->SetText(TEXT("Refresh"));
}

void UFindGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Refresh->OnButtonClicked.AddDynamic(this, &UFindGameWidget::OnRefreshButton_Clicked);
	CheckBox_EnableLan->OnCheckStateChanged.AddDynamic(this, &UFindGameWidget::OnCheckState_Changed);
}

void UFindGameWidget::OnRefreshButton_Clicked()
{
	ScrollBox_Game->ClearChildren();

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsUseLAN; // LAN 기반 검색 (Steam/EOS는 false)
			SessionSearch->MaxSearchResults = 50;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

			SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
							FOnFindSessionsCompleteDelegate::CreateUObject(this, &UFindGameWidget::OnFindSessionsComplete)
						);
			
			SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UFindGameWidget::OnCheckState_Changed(bool IsChecked)
{
	bIsUseLAN = IsChecked;
}

void UFindGameWidget::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid() && SessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("FindSessions Complete: %d results found."), SessionSearch->SearchResults.Num());			
			APlayerController* PC = GetOwningPlayer(); // 올바른 Outer 설정

			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				UServerBrowserItemWidget* ItemWidget = CreateWidget<UServerBrowserItemWidget>(PC, ServerBrowserItemWidgetClass);      

				if (!ItemWidget) // nullptr 체크
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to create ItemWidget!"));
					continue;
				}

				ItemWidget->SetSession(SearchResult); // ✅ SearchResult 복사해서 전달
				ScrollBox_Game->AddChild(ItemWidget);
			}
		}
	}
}
