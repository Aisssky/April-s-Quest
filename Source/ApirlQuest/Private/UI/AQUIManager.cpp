#include "UI/AQUIManager.h"
#include "UI/AQBasePanel.h"
#include "Engine/GameInstance.h"

void UAQUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("AQUIManager Initialized"));
}

void UAQUIManager::Deinitialize()
{
	CloseAllPanels();
	Super::Deinitialize();

}
APlayerController* UAQUIManager::GetOwingPlayerController() const
{

	// 获取当前游戏实例的第一个本地玩家控制器
	UGameInstance* GI = GetGameInstance();
	if (!GI)return nullptr;
	return GI->GetFirstLocalPlayerController();
}

UAQBasePanel* UAQUIManager::OpenPanel(TSubclassOf<UAQBasePanel> PanelClass,int32 Zorder)
{
	if (!PanelClass)return nullptr;

	//检查是否已存在
	for(UAQBasePanel* Panel : PanelStack)
	{
		if(Panel && Panel->GetClass() == PanelClass)
		{
			//已存在，调用open刷新显示
			Panel->OnOpen();
			return Panel;
		}
	}


	//不存在 创建新界面实例
	APlayerController* PC = GetOwingPlayerController();
	if (!PC)return nullptr;

	//add to viewport
	UAQBasePanel* NewPanel = CreateWidget<UAQBasePanel>(PC, PanelClass);
	if (!NewPanel)return nullptr;

	NewPanel->AddToViewport(Zorder);

	//s_push
	PanelStack.Add(NewPanel);

	NewPanel->OnOpen();

	return NewPanel;
}

void UAQUIManager::ClosePanel(TSubclassOf<UAQBasePanel> PanelClass)
{
	if (!PanelClass)return;

	for(int32 i=PanelStack.Num()-1;i>=0;i--)
	{
		UAQBasePanel* Panel = PanelStack[i];
		if(Panel && Panel->GetClass() == PanelClass)
		{
			//找到界面，调用close并从栈中移除
			Panel->OnClose();
			PanelStack.RemoveAt(i);
			return;
		}
	}
}

void UAQUIManager::CloseTopPanel()
{
	if (PanelStack.Num() == 0)return;
	
	UAQBasePanel* TopPanel = PanelStack.Last();
	if (TopPanel)
	{
		TopPanel->OnClose();
	}
	PanelStack.Pop();
}

void UAQUIManager::CloseAllPanels()
{
	//从栈顶开始关闭所有界面
	for (int32 i = PanelStack.Num() - 1; i >= 0; i--)//反向遍历，避免删除元素导致索引问题
	{
		if(PanelStack[i])
		{
			PanelStack[i]->OnClose();
		}
	}
	PanelStack.Empty();
}

UAQBasePanel* UAQUIManager::GetPanel(TSubclassOf<UAQBasePanel> PanelClass)
{
	for(UAQBasePanel* Panel:PanelStack)
	{
		if(Panel && Panel->GetClass() == PanelClass)
		{
			return Panel;
		}
	}
	return nullptr;
}