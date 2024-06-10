// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnerActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API AAISpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawnerActor();

    virtual void Tick(float DeltaTime) override;
    
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AISpawnerSettings")
	TArray<TSubclassOf<class APawn>> SpawnPawnClasses;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AISpawnerSettings")
    float AISpawnInterval = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AISpawnerSettings")
    float AISpawnRadius = 1000.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AISpawnerSettings")
    float TimeToNextWave = 10.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AISpawnerSettings")
    bool bWaitWhileAIDead = false;

    UPROPERTY()
    FTimerHandle AIRespawnTimerHandle;

    UPROPERTY()
    TArray<APawn*> SpawnedAIs;

protected:
	virtual void BeginPlay() override;

    bool bAISpawnActive = false;

    UPROPERTY()
    FTimerHandle AISpawnTimerHandle;

public:
    UFUNCTION(BlueprintCallable)
    void SpawnWave();

    UFUNCTION(BlueprintCallable)
    APawn* SpawnAIPawn(TSubclassOf<class APawn> PawnClass);

    UFUNCTION(BlueprintCallable)
    void SpawnAIPawnByTimer();

    UFUNCTION()
    void OnSpawnedAIDestroyed(AActor* DestroyedActor);

    UFUNCTION()
    void RespawnAI();

    UFUNCTION()
    FTransform GetSpawnAITransform();

private:
    UPROPERTY()
    TArray<TSubclassOf<class APawn>> TempSpawnPawnClasses;
};
