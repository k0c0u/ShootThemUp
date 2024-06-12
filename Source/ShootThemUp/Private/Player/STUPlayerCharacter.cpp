// Shoot Them Up Game. All rights reserved


#include "Player/STUPlayerCharacter.h"

#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    
    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);
    CameraCollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionComponentBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionComponentEndOverlap);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ThisClass::OnStartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);

    if (IsRunning() && WeaponComponent->IsFiring())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnStartRunning()
{
    WantsToRun = true;
    if (IsRunning())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::OnStopRunning()
{
    WantsToRun = false;
}

void ASTUPlayerCharacter::OnStartFire()
{
    if (IsRunning()) return;
    WeaponComponent->StartFire();
}

void ASTUPlayerCharacter::OnCameraCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const bool HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshCheldren;
    GetMesh()->GetChildrenComponents(true, MeshCheldren);

    for(auto MeshChild : MeshCheldren)
    {
        if(const auto MeshChildComponent = Cast<UPrimitiveComponent>(MeshChild))
        {
            MeshChildComponent->SetOnlyOwnerSee(HideMesh);
        }
    }
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

FRotator ASTUPlayerCharacter::GetPlayerControlRotation()
{
    if(HasAuthority() || IsLocallyControlled())
    {
        if(IsValid(GetController()))
        {
            AimRotation = GetController()->GetControlRotation();
        }
    }
    return AimRotation;
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    //PlayAnimMontage(DeathAnimMontage);
    
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASTUPlayerCharacter, AimRotation);
}