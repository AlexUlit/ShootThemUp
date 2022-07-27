// Shoot Them Up Game, All Rights Reserved


#include "Components/STUWeaponComponent.h"

#include "Player/STUBaseCharacter.h"
#include "Weapon/STUBaseWeapon.h"


// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::Fire()
{
	if(!CurrentWeapon) return;

	CurrentWeapon->Fire();
}


// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
	if(!GetWorld()) return;

	ACharacter* Character = Cast<ASTUBaseCharacter>(GetOwner());
	if (!Character) return;
	
	CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
	if (!CurrentWeapon) return;
		
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
	CurrentWeapon->AttachToComponent( Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
}

