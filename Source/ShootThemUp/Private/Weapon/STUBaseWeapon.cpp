// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon,All , All);
// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
	UE_LOG(LogBaseWeapon,Display,TEXT("Fire!"));
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}



