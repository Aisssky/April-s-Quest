#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AQBTService_CheckAttackRange.generated.h"


UCLASS()
class APIRLQUEST_API UAQBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
public:
	UAQBTService_CheckAttackRange();
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//攻击范围
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	//key
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsInAttackRangeKey;
};
