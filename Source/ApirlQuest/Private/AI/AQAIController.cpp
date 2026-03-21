#include "AI/AQAIController.h"
#include "AI/AQEnemyCharacterBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


const FName AAQAIController::TargetActorKey(TEXT("TargetActor"));
const FName AAQAIController::CanSeeTargetKey(TEXT("CanSeeTarget"));

AAQAIController::AAQAIController()
{
	//创建感知组件
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	//创建视觉感知配置
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	//设置视觉感知参数
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;

	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//将视觉感知配置添加到感知组件中
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	//绑定感知更新事件
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAQAIController::OnTargetPerceptionUpdated);


}


void AAQAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AAQEnemyCharacterBase* Enemy = Cast<AAQEnemyCharacterBase>(InPawn);

	if (!Enemy)return;

	if (!Enemy->BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s has no behavior tree!"), *Enemy->GetName());
		return;
	}

	//初始化黑板
	RunBehaviorTree(Enemy->BehaviorTree);

	//测试
	UE_LOG(LogTemp, Warning, TEXT("AIController: PerceptionComponent = %s"),
		AIPerceptionComponent ? TEXT("Valid") : TEXT("NULL"));

	UE_LOG(LogTemp, Warning, TEXT("AIController: 注册感知数量 = %d"),
		AIPerceptionComponent ? AIPerceptionComponent->GetDominantSense() != nullptr ? 1 : 0 : -1);
}

void AAQAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("AIController: 感知到 %s, 成功=%s"),
		*Actor->GetName(),
		Stimulus.WasSuccessfullySensed() ? TEXT("true") : TEXT("false"));

	// Actor 可能为空，先判空
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController: 感知到的 Actor 为空"));
		return;
	}

	//更新黑板中的目标信息
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

	if (!BlackboardComp) {
		UE_LOG(LogTemp, Warning, TEXT("AIController: Blackboard 为空"));
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		//看到目标
		BlackboardComp->SetValueAsObject(TargetActorKey, Actor);
		BlackboardComp->SetValueAsBool(CanSeeTargetKey, true);

		//测试
		UE_LOG(LogTemp, Warning, TEXT("AIController: 设置 TargetActor = %s"), *Actor->GetName());
	}
	else
	{
		//丢失目标
		//BlackboardComp->ClearValue(TargetActorKey);
		BlackboardComp->SetValueAsBool(CanSeeTargetKey, false);

		//测试
		UE_LOG(LogTemp, Warning, TEXT("AIController: 清除 TargetActor"));
	}
}
