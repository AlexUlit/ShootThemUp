// Shoot Them Up Game, All Rights Reserved


#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);



USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
	
	AActor *ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
	}
}

void USTUHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;;

	SetHealth(Health-Damage);
	
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
	
	if(IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(RegenerationTimer,this, &USTUHealthComponent::Regeneration,
			HealUpdateTimer,true, HealDelay);
	}

	
}

void USTUHealthComponent::Regeneration()
{
	SetHealth(Health+HealModifier);
	
	if (FMath::IsNearlyEqual(Health ,MaxHealth) &&GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
	}
}

void USTUHealthComponent::SetHealth(float NewValue)
{
	Health = FMath::Clamp(NewValue, 0.0f ,MaxHealth);
	OnHealthChanged.Broadcast(Health);
	
}




