// Shoot Them Up Game. All rights reserved


#include "AI/AISpawnerActor.h"
#include "NavigationSystem.h"

AAISpawnerActor::AAISpawnerActor()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AAISpawnerActor::BeginPlay()
{
	Super::BeginPlay();

    SpawnWave();
}

void AAISpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAISpawnerActor::SpawnWave()
{
    if (bAISpawnActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cant start spawn AI, because AI Spawn active"));
        return;
    }

    TempSpawnPawnClasses = SpawnPawnClasses;

    if (AISpawnInterval >= 0.1f)
    {
        if (UWorld* WorldPtr = GetWorld())
        {
            bAISpawnActive = true;
            WorldPtr->GetTimerManager().ClearTimer(AISpawnTimerHandle);
            WorldPtr->GetTimerManager().SetTimer(AISpawnTimerHandle, this, &AAISpawnerActor::SpawnAIPawnByTimer, AISpawnInterval, true);
        }
    }
    else
    {
        for (int32 i = TempSpawnPawnClasses.Num() - 1; i >= 0; i--)
        {
            APawn* SpawnedAIPawn = SpawnAIPawn(TempSpawnPawnClasses[i]);
            SpawnedAIPawn->OnDestroyed.AddDynamic(this, &AAISpawnerActor::OnSpawnedAIDestroyed);
            SpawnedAIs.Add(SpawnedAIPawn);

            TempSpawnPawnClasses.RemoveAt(i);
        }

        if (!bWaitWhileAIDead)
        {
            if (UWorld* WorldPtr = GetWorld())
            {
                WorldPtr->GetTimerManager().SetTimer(AIRespawnTimerHandle, this, &AAISpawnerActor::RespawnAI, TimeToNextWave, false);
            }
        }
    }
}

APawn* AAISpawnerActor::SpawnAIPawn(TSubclassOf<class APawn> PawnClass)
{
   if (IsValid(PawnClass))
    {
        if (UWorld* WorldPtr = GetWorld())
        {
            FTransform SpawnAITransform = GetSpawnAITransform();
            if (APawn* SpawnedAIPawn = WorldPtr->SpawnActorDeferred<APawn>(PawnClass, SpawnAITransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
            {
                SpawnedAIPawn->FinishSpawning(SpawnAITransform);

                if (IsValid(SpawnedAIPawn))
                {
                    SpawnedAIPawn->SpawnDefaultController();
                    return SpawnedAIPawn;
                }
            }
        }
    }
    return nullptr;
}

void AAISpawnerActor::OnSpawnedAIDestroyed(AActor* DestroyedActor)
{
    if (SpawnedAIs.Contains(DestroyedActor))
    {
        SpawnedAIs.Remove(Cast<APawn>(DestroyedActor));
    }

    if (SpawnedAIs.IsEmpty())
    {
        if (bWaitWhileAIDead)
        {
            if (UWorld* WorldPtr = GetWorld())
            {
                WorldPtr->GetTimerManager().SetTimer(AIRespawnTimerHandle, this, &AAISpawnerActor::RespawnAI, TimeToNextWave, false);
            }
        }
    }
}

void AAISpawnerActor::RespawnAI()
{
    bAISpawnActive = false;
    SpawnWave();
}

FTransform AAISpawnerActor::GetSpawnAITransform()
{
    FTransform SpawnAITransform = GetActorTransform();

    if (UWorld* WorldPtr = GetWorld())
    {
        if (UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(WorldPtr->GetNavigationSystem()))
        {
            FNavLocation RandomNavLocation;
            if (NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), AISpawnRadius, RandomNavLocation))
            {
                SpawnAITransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f), RandomNavLocation.Location);
            }
        }
    }

    return SpawnAITransform;
}
void AAISpawnerActor::SpawnAIPawnByTimer()
{
    int32 LastIndexToSpawn = TempSpawnPawnClasses.Num() - 1;
    if (TempSpawnPawnClasses.IsValidIndex(LastIndexToSpawn))
    {
        if (IsValid(TempSpawnPawnClasses[LastIndexToSpawn]))
        {
            APawn* SpawnedAIPawn = SpawnAIPawn(TempSpawnPawnClasses[LastIndexToSpawn]);
            SpawnedAIPawn->OnDestroyed.AddDynamic(this, &AAISpawnerActor::OnSpawnedAIDestroyed);
            SpawnedAIs.Add(SpawnedAIPawn);
        }

        TempSpawnPawnClasses.RemoveAt(LastIndexToSpawn);
    }

    if (TempSpawnPawnClasses.IsEmpty())
    {
        bAISpawnActive = false;

        if (UWorld* WorldPtr = GetWorld())
        {
            WorldPtr->GetTimerManager().ClearTimer(AISpawnTimerHandle);

            if (!bWaitWhileAIDead)
            {
                WorldPtr->GetTimerManager().ClearTimer(AIRespawnTimerHandle);
                WorldPtr->GetTimerManager().SetTimer(AIRespawnTimerHandle, this, &AAISpawnerActor::RespawnAI, TimeToNextWave, false);
            }
        }
    }
}
