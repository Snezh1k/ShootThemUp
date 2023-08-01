// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/STUFindEnemyService.h"

#include "AIController.h"
#include "STUAIPerceptionComponent.h"
#include "STUUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionController = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
		if(PerceptionController)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionController->GetClosesEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
