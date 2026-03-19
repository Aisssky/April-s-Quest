#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AQBasePanel.generated.h"


/**
 * 基础行为（显示/隐藏）
 */
UCLASS(Abstract)
class APIRLQUEST_API UAQBasePanel : public UUserWidget
{
	GENERATED_BODY()
public:

	//UIManager调用的接口
	//C++提供默认实现，BP可以重写
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	/*
	    BlueprintNativeEvent` 让一个函数同时具备两种能力 : C++ 提供默认实现（_Implementation 后缀）蓝图可以选择重写，也可以不重写
		如果蓝图重写了，调用蓝图版本如果蓝图没重写，调用 C++ 默认版本
	*/
	void OnOpen();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnClose();	

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnRefresh();

	UFUNCTION(BlueprintPure, Category = "UI")//查询函数
	bool IsOpen() const { return bIsOpen; };

protected:


	virtual void OnOpen_Implementation();
	virtual void OnClose_Implementation();
	virtual void OnRefresh_Implementation();

	bool bIsOpen = false;

};
