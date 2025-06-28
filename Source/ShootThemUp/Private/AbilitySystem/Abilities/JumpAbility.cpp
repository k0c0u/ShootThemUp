// Shoot Them Up Game. All rights reserved


#include "AbilitySystem/Abilities/JumpAbility.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UJumpAbility::UJumpAbility()
{
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UJumpAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
   if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
   {
       return false;
   }

    const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(),ECastCheckedType::NullAllowed );
    return Character->CanJump();
}

void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            return;
        }
        
        Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
        
        ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
        Character->Jump();
    }
}