#include "Block.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "OrionixCharacter.h"

// Constructor
ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;								// The tick function can be called in every frame

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));		// Creates a UStaticMeshComponent and names it
	RootComponent = meshComponent;									// Sets meshComponent as RootComponent

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));				// Creates a UBoxComponent and names it
	triggerBox->SetupAttachment(RootComponent);							// Attachs the triggerBox to the RootComponent
	triggerBox->SetBoxExtent(FVector(250.0f, 10.0f, 250.0f));					// Sets the size of the triggerBox
	triggerBox->SetRelativeLocation(FVector(250.0f, 0.0f, 250.0f));					// Sets the relative location of the triggerBox
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);					// Only enable query collisions
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);					// Close all collision channels
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);				// Open collision channel for characters only
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABlock::onTriggerBoxOverlap);		// Registers the function to be called during collisions with the triggerBox

	hideAndDisableCollision();									// Sets the block as abstract
}

// Protected Functions
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlock::onTriggerBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor && OtherActor->IsA(AOrionixCharacter::StaticClass()) && flag == false)
	{
		onBlockTriggered.Broadcast();								// Broadcasts the onBlockTriggered event to all subscribers.
		flag = true;
	}
}

// Public Functions
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLocation = GetActorLocation();
	currentLocation = currentLocation + (platformVelocity * DeltaTime);
	SetActorLocation(currentLocation);
}

void ABlock::setBlockMesh(const FString &meshPath)
{
	UStaticMesh *mesh = LoadObject<UStaticMesh>(nullptr, *meshPath);
	if(mesh)
	{
		meshComponent->SetStaticMesh(mesh);
	}
}

void ABlock::rotateBlockRandomly()
{
	int32 rotationDegree = FMath::RandRange(0, 3) * 90;						// Generates random values between 0-3 and multiplies by 90.
	setMeshRotation(rotationDegree);
}

void ABlock::setBlockLocation(const FVector &newLocation)
{
	SetActorRelativeLocation(newLocation);
}

void ABlock::hideAndDisableCollision()
{
	SetActorHiddenInGame(true);									// Hides actor
	SetActorEnableCollision(false);									// Disables collision 
}

void ABlock::unhideAndEnableCollision()
{
	SetActorHiddenInGame(false);									// Shows actor
	SetActorEnableCollision(true);									// Enables collision
}

// Private Functions
void ABlock::rotateAroundCenter(float rotationValue)
{
	FRotator currentRotation = meshComponent->GetComponentRotation();				// Get current rotation
	FRotator deltaRotation(rotationValue, 0.0f, 0.0f);						// Create delta rotation vector
	FRotator newRotation = currentRotation + deltaRotation;						// Calculate new rotation

	FBoxSphereBounds bounds = meshComponent->CalcBounds(FTransform::Identity);			// Calculates the meshComponent's bounding box and sphere
	FVector newCenter = bounds.Origin + deltaRotation.RotateVector(-bounds.Origin);			// Calculates new center of meshComponent

	meshComponent->SetWorldLocation(newCenter);							// Set meshComponent location
	meshComponent->SetWorldRotation(newRotation);							// Set meshComponent rotation
}

void ABlock::setMeshRotation(float rotationValue)
{
	FRotator NewRotation = FRotator(rotationValue, 0, 0);						// Create rotation vector
	SetActorRotation(NewRotation);
}

