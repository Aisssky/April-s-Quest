#include "UI/AQBasePanel.h"

void UAQBasePanel::OnOpen_Implementation()
{
	bIsOpen = true;
	SetVisibility(ESlateVisibility::Visible);
}

void UAQBasePanel::OnClose_Implementation()
{
	bIsOpen = false;
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAQBasePanel::OnRefresh_Implementation()
{
	//默认不做任何刷新
}

