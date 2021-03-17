// Shoot Them Up Game. All rights reserved


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"


ASTUProjectile::ASTUProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);


}


void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}


