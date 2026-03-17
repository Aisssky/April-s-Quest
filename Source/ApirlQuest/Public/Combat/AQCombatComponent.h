#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AQCombatComponent.generated.h"
/**
 * 
 */
UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class APIRLQUEST_API UAQCombatComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UAQCombatComponent();
};
