// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASTUBasePickup();
    
    virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

    bool CouldBeTaken() const;


protected:
	
	virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f;

private:
    
    UPROPERTY()
    TArray<APawn*> OverlappingPawns;

    float RotationYaw = 0.0f;
    FTimerHandle RespawnTimerHandle;
    
    virtual bool GivePickupTo(APawn* PlayerPawn);
    void PickupWasTaken();
    void Respawn();
    void GenerateRotationYaw();

    
};
