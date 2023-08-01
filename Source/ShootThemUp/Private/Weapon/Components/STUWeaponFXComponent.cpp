// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponentPool.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;
	if(Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if(ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}
	//Niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	//Decal
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		ImpactData.DecalData.Material,
		ImpactData.DecalData.Size,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation());
	if(DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}
	//Sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}

void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTUWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

