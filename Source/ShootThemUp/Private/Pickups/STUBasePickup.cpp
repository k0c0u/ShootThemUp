// Shoot Them Up Game. All rights reserved


#include "Pickups/STUBasePickup.h"

#include "Animation/AnimInstanceProxy.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

}

void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
    GenerateRotationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));

    for (const auto OverlapPawn : OverlappingPawns)
    {
        if (GivePickupTo(OverlapPawn))
        {
            PickupWasTaken();
            break;
        }
    }
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    
    const auto Pawn = Cast<APawn>(OtherActor);
    
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
    else if (IsValid(Pawn))
    {
        OverlappingPawns.Add(Pawn);
    }
}

void ASTUBasePickup::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);
    
    const auto Pawn = Cast<APawn>(OtherActor);
    OverlappingPawns.Remove(Pawn);
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    if(IsValid(GetRootComponent()))
    {
        GetRootComponent()->SetVisibility(false, true);
    }
   
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw();
    if(IsValid(GetRootComponent()))
    {
        GetRootComponent()->SetVisibility(true, true);
    }
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUBasePickup::GenerateRotationYaw()
{
    const float Directional = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw = FMath::RandRange(1.0f, 2.0f) + Directional;
}

bool ASTUBasePickup::CouldBeTaken() const
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}