// Shoot Them Up Game, All Rights Reserved


#include "Player/STUBaseCharacter.h"

#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit):
	Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MySpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("FuckingSpringArmComponent");
	MySpringArmComponent->SetupAttachment(GetRootComponent());
	MySpringArmComponent->bUsePawnControlRotation = true;
	MySpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);
	

	MyCameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	MyCameraComponent->SetupAttachment(MySpringArmComponent);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthAmount");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
	
	
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement())
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnPlayerDead);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::CharacterHealthChanged);
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnMyGrounded);
	
}




void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto Health = HealthComponent->getHealth();
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
	
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	InputComponent->BindAxis("MoveForward",this, &ASTUBaseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight",this, &ASTUBaseCharacter::MoveRight);
	InputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	InputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::AddControllerYawInput);
	InputComponent->BindAction("ABJump",IE_Pressed,this, &ASTUBaseCharacter::Jump);
	InputComponent->BindAction("ABSprint", IE_Pressed, this, &ASTUBaseCharacter::OnStartSprint);
	InputComponent->BindAction("ABSprint", IE_Released, this, &ASTUBaseCharacter::OnStopSprint);
	InputComponent->BindAction("ABFire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);
}

bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun&&IsMovingForward&&!GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
	if(GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees: Degrees*FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount>0.0f;
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(),Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartSprint()
{
	WantsToRun = true;
}

void ASTUBaseCharacter::OnStopSprint()
{
	WantsToRun = false;
}

void ASTUBaseCharacter::OnPlayerDead()
{
	UE_LOG(LogBaseCharacter, Error, TEXT("Player %s is Dead"), *GetName());
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ASTUBaseCharacter::CharacterHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnMyGrounded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(LogBaseCharacter, Display, TEXT ("On landed: %f"), FallVelocityZ);
	if (FallVelocityZ<LandedDamageVelocity.X) return;
	auto const FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity,LandedDamage,
		FallVelocityZ);
	UE_LOG(LogBaseCharacter, Display, TEXT ("On landed damage is: %f"), FinalDamage);
	TakeDamage(FinalDamage,FDamageEvent{},nullptr,nullptr);
}


