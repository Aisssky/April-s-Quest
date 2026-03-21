#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AQAIController.generated.h"

/**
 * 
 */

UCLASS()
class APIRLQUEST_API AAQAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAQAIController();

	//AIController接口
	virtual void OnPossess(APawn* InPawn) override;

	//blackboard key
	static const FName TargetActorKey; //攻击目标
	static const FName CanSeeTargetKey;//是否能看到攻击目标

protected:
	UPROPERTY(VisibleAnywhere,  Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	//感知到目标时调用
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
