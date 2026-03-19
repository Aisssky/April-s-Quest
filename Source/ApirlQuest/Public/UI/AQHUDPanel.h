#pragma once

#include "CoreMinimal.h"
#include "UI/AQBasePanel.h"
#include "AbilitySystem/AQAttributeSet.h"
#include "AQHUDPanel.generated.h"

class UProgressBar;
class UTextBlock;
class AAQCharacterBase;
class UAQAttributeSet;

UCLASS(Blueprintable)
class APIRLQUEST_API UAQHUDPanel : public UAQBasePanel
{
	GENERATED_BODY()
public:

	//委托回调函数
	UFUNCTION()
	void OnHealthChanged(float NewVaule,float MaxVaule);

	UFUNCTION()
	void OnStaminaChanged(float NewVaule, float MaxVaule);

	//UI可绑定的控件
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Stamina;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_HealthText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TB_StaminaText;

protected:

	//生命周期
	//beginplay
	virtual void NativeConstruct() override;
	//widget销毁 解绑委托 防止野指针回调
	virtual void NativeDestruct() override;


	//辅助函数
private:

	void BindToPlayerCharacter();


	//弱指针 Character死亡后变成nullptr，防止野指针回调崩溃
	TWeakObjectPtr<UAQAttributeSet> CachedAttributeSet;


};
