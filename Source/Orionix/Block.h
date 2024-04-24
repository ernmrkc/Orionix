#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Block.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockTriggered);			// Defines a delegate type that can be dynamically bound to multiple functions, allowing for event broadcasting in Blueprint.

UCLASS()
class ORIONIX_API ABlock: public AActor
{
	GENERATED_BODY()

	/**
	 * **FUNCTION DECLARATIONS**
	 */
public:
	/**
	 * @brief			Constructor of Block class
	 */
	ABlock();

	/**
	 * @brief			Called every frame.
	 *				Use for update actor's position, check collision etc.
	 * @param DeltaTime		Time elapsed since the last frame, in seconds.
	 */
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * @brief			Sets the static mesh of the meshComponent.
	 * @param MeshPath		Path of staticMesh to set.
	 */
	void setBlockMesh(const FString &MeshPath);

	/**
	 * @brief			Rotates the block to a random orientation around its X-axis.
	 *				This function seletcs a random rotation from {0, 90, 180, 270} degrees and applies it, and
	 *				allowing for varied visual appearances of the block in the game world.
	 */
	void rotateBlockRandomly();

	/**	
	 * @brief			Sets the actor's RootComponent to the specified relative location.
	 * @param newLocation		New relative location of the actor's root component.
	 */
	void setBlockLocation(const FVector &newLocation);

	/**
	 * @brief			Hides the actor in the game and disables its collision.
	 */
	void hideAndDisableCollision();

	/**
	 * @brief			Makes the actor visible in the game and enables its collision.
	 */
	void unhideAndEnableCollision();

protected:
	/**
	 * @brief			Called when the game starts or when spawned.
	 *				Use this function to initialize any variables, set up game components, or perform actions that need to occur right as the game begins.
	 */
	virtual void BeginPlay() override;


	UFUNCTION()
	/**
	 * @brief			Called when another component begins overlapping this block's trigger box.
	 * @param OverlappedComponent	The component that triggered the overlap event.
	 * @param OtherActor		The actor that started to overlap with this block.
	 * @param OtherComp		The component of the other actor thar overlaps with this block.
	 * @param OtherBodyIndex	The body index of the overlapping component, if applicable.
	 * @param bFromSweep		Indicates if the overlap was the result of a sweep.
	 * @param SweepResult		Provides detailed information about the sweep, if bFromSweep is true.
	 */
	void onTriggerBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	/**
	 * @brief			**DEPRECATED**
	 *				Rotates the block around its geometric center along the X-axis.
	 * @param rotationValue		The rotation angle in degrees to be applied to the actor's X-axis.
	 */
	void rotateAroundCenter(float rotationValue);		

	/**
	 * @brief			Sets the rotation of the mesh component of the actor.
	 * @param rotationValue 	The rotation angle in degrees to be applied to the actor's X-asis.
	 */
	void setMeshRotation(float rotationValue);			

	/**
	 * **VARIABLE DECLARATIONS**
	 */
public:
	UPROPERTY(VisibleAnywhere)								// It is always visible in the game editor but cannot be changed.
	UStaticMeshComponent *meshComponent;							// Represents visual appearance of the block in the game world.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")			// Always visible, cannot be changed, readable but not writeable in Blueprints, listed in the Trigger category.
	UBoxComponent *triggerBox;								// Represents the end of the block.

	UPROPERTY(BlueprintAssignable)				// Allows the property to be bound to events in Blueprints, enabling custom response to be defined in the Blueprint editor.
	FOnBlockTriggered onBlockTriggered;			// An event that can be broadcasted to notify other classes or Blueprints when a specific action occurs, allowing for custom event handling.

	bool flag = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Platform Velocity")
	FVector platformVelocity = FVector(500, 0, 0);
};
