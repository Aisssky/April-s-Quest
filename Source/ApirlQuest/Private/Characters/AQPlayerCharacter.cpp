#include "Characters/AQPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystem/AQAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include  "EnhancedInputSubsystems.h"	
#include "UI/AQUIManager.h"


AAQPlayerCharacter::AAQPlayerCharacter()
{
	//创建相机组件
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//旋转
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AAQPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	// HUD 
	if (HUDPanelClass)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UAQUIManager* UIManager = GI->GetSubsystem<UAQUIManager>())
			{
				UIManager->OpenPanel(HUDPanelClass, 0);
			}
		}
	}

}


void AAQPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAQPlayerCharacter::HandleMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAQPlayerCharacter::HandleLook);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AAQPlayerCharacter::HandleAttack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AAQPlayerCharacter::HandleDodge);
	}
}

void AAQPlayerCharacter::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller)return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, MovementVector.Y);
	AddMovementInput(RightDir, MovementVector.X);
}

void AAQPlayerCharacter::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AAQPlayerCharacter::HandleAttack()
{
	//解耦
	if (AbilitySystemComponent && AttackAbilityTag.IsValid())
	{
		AbilitySystemComponent->TryActivateAbilityByTag(AttackAbilityTag);
	}
}

void AAQPlayerCharacter::HandleDodge()
{
	if (AbilitySystemComponent && DodgeAbilityTag.IsValid())
	{
		AbilitySystemComponent->TryActivateAbilityByTag(DodgeAbilityTag);
	}
}
