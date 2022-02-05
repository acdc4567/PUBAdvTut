

#include "FPSArmsAnimInstance.h"
#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UFPSArmsAnimInstance::UFPSArmsAnimInstance(){

}

void UFPSArmsAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	}
    if (ShooterCharacter)
	{
        FVector Velocity { ShooterCharacter->GetVelocity() };
        Velocity.Z = 0;
        Speed = Velocity.Size();
        
        bIsAiming=ShooterCharacter->GetIsAiming();


    }
}


void UFPSArmsAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<ASCharacter>(TryGetPawnOwner());
}









