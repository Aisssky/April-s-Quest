#include "AI/AQEnemyCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"


AAQEnemyCharacterBase::AQEnemyCharacterBase()
{
	//AIcontroller获取
	EAutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
