// Shoot Them Up Game. All rights reserved


#include "GameUtils/GameUtilsLibrary.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UGameUtilsLibrary::AsyncSpawnActor(UObject* WorldContextObject, TSoftClassPtr<AActor> Asset, FTransform SpawnTransform, const FAsyncSpawnActorDelegate& Callback)
{
    FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
    const FSoftObjectPath& Target = Asset.ToSoftObjectPath();
    AssetLoader.RequestAsyncLoad(Target, FStreamableDelegate::CreateStatic(&UGameUtilsLibrary::OnAsyncSpawnActorComplete, WorldContextObject, Target, SpawnTransform, Callback));
}

void UGameUtilsLibrary::OnAsyncSpawnActorComplete(UObject* WorldContextObject, FSoftObjectPath Reference, FTransform SpawnTransform, FAsyncSpawnActorDelegate Callback)
{
    if (!IsValid(WorldContextObject->GetWorld()))
    {
        return;
    }

    AActor* SpawnedActor = nullptr;

    if (UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *(Reference.ToString()))))
    {

        FActorSpawnParameters Params;
        Params.Owner = Cast<AActor>(WorldContextObject);
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, Params);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AMLXBotSpawner::OnAsyncSpawnActorComplete -- Failed to load object: $"), *Reference.ToString());
    }
    Callback.ExecuteIfBound(SpawnedActor != nullptr, Reference, SpawnedActor);
}