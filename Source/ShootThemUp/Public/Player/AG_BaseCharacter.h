// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "GameTypes.h"
#include "InputActionValue.h"
#include "AG_BaseCharacter.generated.h"


class UAG_AbilitySystemComponentBase;
class UAG_AttributeSetBase;

class UGameplayEffect;
class UGameplayAbility;

class UInputAction;


UCLASS()
class SHOOTTHEMUP_API AAG_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAG_BaseCharacter(const FObjectInitializer& ObjInit);
	
    virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;
    
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PawnClientRestart() override;

    virtual void Landed(const FHitResult& Hit) override;

    bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext);

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UFUNCTION(BlueprintCallable)
    FCharacterData GetCharacterData() const {return CharacterData;}

    UFUNCTION(BlueprintCallable)
    void SetCharacterData(const FCharacterData& Data);

    
    
protected:
	virtual void BeginPlay() override;
    
    void GiveAbilities();
    void ApplyStartupEffects();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    virtual void InitFromCharacterData(const FCharacterData& Data, bool bFromReplication = false);

    UFUNCTION()
    void OnRep_CharacterData();

    //Input
    void OnMoveForward(const FInputActionValue& Value);
    void OnMoveSide(const FInputActionValue& Value);
    void OnLookUp(const FInputActionValue& Value);
    void OnTurn(const FInputActionValue& Value);
    
    void OnJumpStarted(const FInputActionValue& Value);
    void OnJumpEnded(const FInputActionValue& Value);

    void OnSprintStarted(const FInputActionValue& Value);
    void OnSprintEnded(const FInputActionValue& Value);

    void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);
protected:

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAG_AbilitySystemComponentBase>  AbilitySystemComponent;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAG_AttributeSetBase> AttributeSet;

    UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
    FCharacterData CharacterData;

    UPROPERTY(EditDefaultsOnly)
    class UCharacterDataAsset* CharacterDataAsset;

    //Input
    UPROPERTY(EditDefaultsOnly)
    class UInputMappingContext* DefaultInputMappingContex;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* MoveForward;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* MoveSide;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* LookUp;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* Turn;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* JumpAction;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag JumpEventTag;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTagContainer InAirTags;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTagContainer SprintTags;
};