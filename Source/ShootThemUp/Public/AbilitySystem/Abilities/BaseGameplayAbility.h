// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"


class AAG_BaseCharacter;
 
UCLASS()
class SHOOTTHEMUP_API UBaseGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    AAG_BaseCharacter* GetCharacterFromActorInfo() const;
protected:
    /** For instance ability*/
    UPROPERTY(EditDefaultsOnly, Category="Effects")
    TArray<TSubclassOf<UGameplayEffect>> OngoingEffectToRemoveOnEnd;

    /** For instance ability*/
    UPROPERTY(EditDefaultsOnly, Category="Effects")
    TArray<TSubclassOf<UGameplayEffect>> OngoingEffectToJustApplyOnStart;

    TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;
};
