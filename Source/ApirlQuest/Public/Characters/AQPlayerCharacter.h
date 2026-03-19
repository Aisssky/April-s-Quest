#pragma once

#include "CoreMinimal.h"
#include "Characters/AQCharacterBase.h"
#include "InputActionValue.h"
#include "UI/AQBasePanel.h"
#include "AQPlayerCharacter.generated.h"


/**
 * 
 */

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;


UCLASS()
class APIRLQUEST_API AAQPlayerCharacter :public AAQCharacterBase
{
	GENERATED_BODY()

public:
	AAQPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UAQBasePanel> HUDPanelClass;

	//相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	//输入
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;

	//输入处理
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleAttack();
	void HandleDodge();

	//技能Tag
	//解耦输入和技能系统，输入只负责触发技能，技能系统根据Tag来判断执行哪个技能
	UPROPERTY(EditDefaultsOnly, Category = "CAS|Tags")
	FGameplayTag AttackAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "CAS|Tags")
	FGameplayTag DodgeAbilityTag;

};
