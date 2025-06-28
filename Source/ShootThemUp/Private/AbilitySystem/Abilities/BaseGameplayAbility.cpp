// Shoot Them Up Game. All rights reserved


#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AG_BaseCharacter.h"

void UBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
    {
        FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
        for (auto GameplayEffect : OngoingEffectToJustApplyOnStart)
        {
            if (!GameplayEffect.Get()) continue;
            FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,1,Context);
            if (SpecHandle.IsValid())
            {
                FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                if (!ActiveGEHandle.WasSuccessfullyApplied())
                {
                    UE_LOG(LogTemp, Error, TEXT("Ability %s failed to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
                }
            }
        }
    
        if (IsInstantiated())
        {
            for (auto GameplayEffect : OngoingEffectToRemoveOnEnd)
            {
                if (!GameplayEffect.Get()) continue;
                FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,1,Context);
                if (SpecHandle.IsValid())
                {
                    FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                    if (!ActiveGEHandle.WasSuccessfullyApplied())
                    {
                        UE_LOG(LogTemp, Error, TEXT("Ability %s failed to apply end effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
                    }
                    else
                    {
                        RemoveOnEndEffectHandles.Add(ActiveGEHandle);
                    }
                }
            }
        }
    }
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (IsInstantiated())
    {
        if (UAbilitySystemComponent* AbilityComponent = ActorInfo->AbilitySystemComponent.Get())
        {
            for (FActiveGameplayEffectHandle GameplayEffectHandle : RemoveOnEndEffectHandles)
            {
                if (GameplayEffectHandle.IsValid())
                {
                    AbilityComponent->RemoveActiveGameplayEffect(GameplayEffectHandle);
                }
            }
            RemoveOnEndEffectHandles.Empty();
        }
    }
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AAG_BaseCharacter* UBaseGameplayAbility::GetCharacterFromActorInfo() const
{
    return Cast<AAG_BaseCharacter>(GetAvatarActorFromActorInfo());
}