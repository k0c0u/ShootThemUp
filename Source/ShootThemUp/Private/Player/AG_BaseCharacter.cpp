// Shoot Them Up Game. All rights reserved


#include "Player/AG_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "STUCharacterMovementComponent.h"
#include "AbilitySystem/Attribute/AG_AttributeSetBase.h"
#include "AbilitySystem/Components/AG_AbilitySystemComponentBase.h"
#include "Data/CharacterDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputTriggers.h"


AAG_BaseCharacter::AAG_BaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	
	PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UAG_AbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UAG_AttributeSetBase>(TEXT("AttributeSet"));
}

void AAG_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &ThisClass::OnMaxMovementSpeedChanged);
}

void AAG_BaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AAG_BaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (IsValid(CharacterDataAsset))
    {
        SetCharacterData(CharacterDataAsset->CharacterData);
    }
}

void AAG_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (IsValid(MoveForward))
        {
            EnhancedInputComponent->BindAction(MoveForward, ETriggerEvent::Triggered, this, &ThisClass::OnMoveForward);
        }

        if (IsValid(MoveSide))
        {
            EnhancedInputComponent->BindAction(MoveSide, ETriggerEvent::Triggered, this, &ThisClass::OnMoveSide);
        }

        if (IsValid(LookUp))
        {
            EnhancedInputComponent->BindAction(LookUp, ETriggerEvent::Triggered, this, &ThisClass::OnLookUp);
        }

        if (IsValid(Turn))
        {
            EnhancedInputComponent->BindAction(Turn, ETriggerEvent::Triggered, this, &ThisClass::OnTurn);
        }

        if (IsValid(JumpAction))
        {
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::OnJumpStarted);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::OnJumpEnded);
        }
        
        if (IsValid(SprintAction))
        {
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::OnSprintStarted);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::OnSprintEnded);
        }
    }
}

void AAG_BaseCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(DefaultInputMappingContex, 0);
        }
    }
}

void AAG_BaseCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->RemoveActiveEffectsWithTags(InAirTags);
    }
}

void AAG_BaseCharacter::GiveAbilities()
{
    if (HasAuthority() && IsValid(AbilitySystemComponent))
    {
        for (auto DefaultAbility : CharacterData.Abilities)
        {
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
        }
    }
}

void AAG_BaseCharacter::ApplyStartupEffects()
{
    if (GetLocalRole() == ROLE_Authority) // On Server
    {
        FGameplayEffectContextHandle EffectContex = AbilitySystemComponent->MakeEffectContext();
        EffectContex.AddSourceObject(this);

        for (auto CharacterEffect : CharacterData.Effects)
        {
            ApplyGameplayEffectToSelf(CharacterEffect, EffectContex);
        }
    }
}

void AAG_BaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    GiveAbilities();
    ApplyStartupEffects();
}

void AAG_BaseCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AAG_BaseCharacter::SetCharacterData(const FCharacterData& Data)
{
    CharacterData = Data;
    InitFromCharacterData(CharacterData);
}

void AAG_BaseCharacter::InitFromCharacterData(const FCharacterData& Data, bool bFromReplication)
{
    
}

void AAG_BaseCharacter::OnRep_CharacterData()
{
    InitFromCharacterData(CharacterData, true);
}

bool AAG_BaseCharacter::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext)
{
    if (!IsValid(Effect))
        return false;
    FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, InEffectContext);
    if (SpecHandle.IsValid())
    {
        FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        return ActiveGEHandle.WasSuccessfullyApplied();
    }
    return false;
}

UAbilitySystemComponent* AAG_BaseCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

//Input
void AAG_BaseCharacter::OnMoveForward(const FInputActionValue& Value)
{
    if (Value.GetMagnitude() == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Value.GetMagnitude());
}

void AAG_BaseCharacter::OnMoveSide(const FInputActionValue& Value)
{
    if (Value.GetMagnitude() == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Value.GetMagnitude());
}

void AAG_BaseCharacter::OnLookUp(const FInputActionValue& Value)
{
    AddControllerPitchInput(Value.GetMagnitude());
}

void AAG_BaseCharacter::OnTurn(const FInputActionValue& Value)
{
    AddControllerYawInput(Value.GetMagnitude());
}

void AAG_BaseCharacter::OnJumpStarted(const FInputActionValue& Value)
{
    FGameplayEventData Payload;
    
    Payload.Instigator = this;
    Payload.EventTag = JumpEventTag;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, JumpEventTag, Payload);
}

void AAG_BaseCharacter::OnJumpEnded(const FInputActionValue& Value)
{
    //StopJumping();
}

void AAG_BaseCharacter::OnSprintStarted(const FInputActionValue& Value)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->TryActivateAbilitiesByTag(SprintTags, true);
    }
}

void AAG_BaseCharacter::OnSprintEnded(const FInputActionValue& Value)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->CancelAbilities(&SprintTags);
    }
}

//InputEnd

void AAG_BaseCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
    GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AAG_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, CharacterData);
}
