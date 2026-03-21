#include "AI/AQEnemyCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTree.h"


AAQEnemyCharacterBase::AAQEnemyCharacterBase()
{
	//AIcontroller获取
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


void AAQEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAQEnemyCharacterBase::PossessedBy(AController* NewController)
{
	//因为继承了CharacterBAse ，所以直接复用
	Super::PossessedBy(NewController);
}
