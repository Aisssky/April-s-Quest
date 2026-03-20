#pragma once

#include "CoreMinimal.h"
#include "Characters/AQCharacterBase.h"
#include "AQEnemyCharacterBase.generated.h"

/**
 * 
 */

class UAIPerceptionComponent;
class UBehaviorTree;

UCLASS()
class APIRLQUEST_API AAQEnemyCharacterBase : public AAQCharacterBase
{
	GENERATED_BODY()

public:
	AAQEnemyCharacterBase();

	//AIcontroller
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

protected:
	virtual void BeginPlay() override;

	//被controller获取后初始化ASC
	virtual void PossessedBy(AController* NewController) override;
};
