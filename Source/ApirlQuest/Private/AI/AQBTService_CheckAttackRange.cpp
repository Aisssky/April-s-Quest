
#include "AI/AQBTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"




UAQBTService_CheckAttackRange::UAQBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");

	//每0.1秒检查一次
	Interval = 0.1f;
}

void UAQBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//获取AI控制器
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	//获取黑板组件
	UBlackboardComponent* BB = Controller->GetBlackboardComponent();
	if (!BB) return;


	//获取AI角色
	APawn* EnemyPawn = Controller->GetPawn();
	if (!EnemyPawn) return;

	//从BB获取目标Actor
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
	if (!Target) {
		BB->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, false);
		return;
	}

	//计算距离
	const float Distance = FVector::Dist(EnemyPawn->GetActorLocation(), Target->GetActorLocation());

	//检查是否在攻击范围内
	const bool bIsInRange = Distance <= AttackRange;

	//更新黑板值
	BB->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bIsInRange);
}
