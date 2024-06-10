// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, Health);
    

	USTUHealthComponent();

	FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnHealthChanged OnFloatHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const {return FMath::IsNearlyZero(Health);}

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const {return Health / MaxHealth;}

    UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealth(float NewHealth);

    bool TryToAddHealth(float HealthAmount);
    bool IsHealthFull()  const;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1000.0"), Category = "Health")
    float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal "))
    float HealUpdateTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal "))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal "))
    float HealModifier = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSoftClassPtr<UCameraShakeBase> CameraShake;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnRep_Health();
    virtual void OnRep_Health_Implementation()
    {
        OnFloatHealthChanged.Broadcast(Health);
    }
    
private:
    UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health = 0.0f;

	FTimerHandle HealTimerHandle;

	void HealUpdate();

	

	UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void PlayCameraShake();

    void Killed(AController* KillerController);
};
