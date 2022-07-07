// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUHealthComponent();
	float getHealth() const {return Health;};

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const {return FMath::IsNearlyZero(Health);};

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.0f, ClampMax =100.0f));
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal");
	bool AutoHeal = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal");
	float HealUpdateTimer = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal");
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal");
	float HealModifier = 1.0f;
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamageHandle(
			AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	void Regeneration();
	FTimerHandle RegenerationTimer;

	void SetHealth(float NewValue);
	
};
