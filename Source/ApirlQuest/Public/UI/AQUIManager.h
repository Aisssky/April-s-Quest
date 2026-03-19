#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "AQUIManager.generated.h"

/**
 *使用栈的方法管理UI界面，打开界面时将界面压入栈顶，关闭界面时将界面弹出栈顶
 */

class UAQBasePanel;


UCLASS()
class APIRLQUEST_API UAQUIManager : public UGameInstanceSubsystem 
{
	GENERATED_BODY()

public:
	//USubSystem 接口，自动调用
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	//打开界面，返回界面实例
	UFUNCTION(BlueprintCallable, Category = "UI")
	UAQBasePanel* OpenPanel(TSubclassOf<UAQBasePanel> PanelClass, int32 ZOrder = 0);

	//关闭界面
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClosePanel(TSubclassOf<UAQBasePanel> PanelClass);

	//ESC关闭栈顶
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseTopPanel();

	//关闭所有界面
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseAllPanels();

	//获取当前已存在的界面
	UFUNCTION(BlueprintCallable, Category = "UI")
	UAQBasePanel* GetPanel(TSubclassOf<UAQBasePanel> PanelClass);

private:

	//界面栈
	UPROPERTY()
	TArray<TObjectPtr<UAQBasePanel>> PanelStack;//Array更灵活

	//获取Controller
	APlayerController* GetOwingPlayerController() const;
};
