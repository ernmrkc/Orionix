#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "FBlockPool.h"
#include "Block.h"
#include "TunnelManager.generated.h"


UCLASS()
class ORIONIX_API ATunnelManager: public AActor
{
	GENERATED_BODY()

	/**
	 * **FUNCTION DECLARATIONS**
	 */
public:
	/**
	 * @brief			Constructor of TunnelManager class
	 */
	ATunnelManager();

	/**
	 * @brief			Desturctor of TunnelManager class
	 */
	virtual ~ATunnelManager();

	/**
	 * @brief			Called every frame.
	 *				Use for update actor's position, check collision etc.
	 * @param DeltaTime		Time elapsed since the last frame, in seconds.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief			Initializes the tunnel with blocks. Up to **maxBlocks** can be added to the tunnel.
	 *				Blocks are added to ArrowComponent because rotation operations are done through ArrowComponent.
	 */
	void initializeTunnel();

	/**
	 * @brief			Subscribes to block trigger events for all blocks in the pool.
	 *				Iterates through the block pool and binds the handleBlockTrigger function to the onBlockTriggered event of each block.
	 */
	void subscribeToBlockEvents();

	/**
	 * @brief			Takes a new block from pool, and adjust its collisions, visibility, etc. settings then adds to the tunnel.
	 *				Each block is added to the ArrowComponent.
	 */
	void addBlockToTunnel();

	/**
	 * @brief			It removes the oldest block in the tunnel and returns it back into the pool.
	 */
	void removeBlockFromTunnel();

	UFUNCTION()
	/**
	 * @brief			Handles a block trigger event by executing a sequence of actions:
	 *				- Adds a new block to the end of the tunnel.
	 *				- Removes the oldest block from the buffer to maintain tunnel length.
	 *				- Moves trigger boxes forward to adjust for the new tunnel configuration.
	 */
	void handleBlockTrigger();

	void turnRight();
	void turnLeft();
	void triggerRandomTurn();
	void onTurnTimerExpired();

protected:
	/**
	 * @brief			Called when the game starts or when spawned.
	 *				Use this function to initialize any variables, set up game components, or perform actions that need to occur right as the game begins.
	 */
	virtual void BeginPlay() override;

	UFUNCTION()
	/**
	 * @brief				Called when the character component starts overlapping the trigger box on the left side of the tunnel.
	 * @param OverlappedComponent		The component that triggered the overlap event.
	 * @param OtherActor			The actor that started to overlap with this block.
	 * @param OtherComp			The component of the other actor thar overlaps with this block.
	 * @param OtherBodyIndex		The body index of the overlapping component, if applicable.
	 * @param bFromSweep			Indicates if the overlap was the result of a sweep.
	 * @param SweepResult			Provides detailed information about the sweep, if bFromSweep is true.
	 */
	void OnTriggerBoxOverlapLeft(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	/**
	 * @brief				Called when the character component starts overlapping the trigger box on the right side of the tunnel.
	 * @param OverlappedComponent		The component that triggered the overlap event.
	 * @param OtherActor			The actor that started to overlap with this block.
	 * @param OtherComp			The component of the other actor thar overlaps with this block.
	 * @param OtherBodyIndex		The body index of the overlapping component, if applicable.
	 * @param bFromSweep			Indicates if the overlap was the result of a sweep.
	 * @param SweepResult			Provides detailed information about the sweep, if bFromSweep is true.
	 */
	void OnTriggerBoxOverlapRight(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	/**
	 * @brief			Generates and configures a pair of trigger boxes (left and right) for the tunnel.
	 */
	void generateTriggerBoxPairs();

	/**
	 * @brief			Adds a new block to the tunnel block buffer.
	 *				This function appends the provided block to the end of the tunnelBlocks TArray.
	 */

	 /**
	  * @brief			Adds a new block to the tunnel block buffer.
	  *				This function appends the provided block to the end of the tunnelBlocks TArray.
	  * @param newBlock		The block to be added to the tunnel.
	  */
	void addBlockToBuffer(ABlock *newBlock);

	/**
	 * @brief			Removes the oldest block from the tunnel block buffer.
	 */
	void removeBlockFromBuffer();

	/**
	 * @brief			Updates the rotation of the ArrowComponent based on current rotation flags and limits.
	 *				If rotation is initiated (left or right) and has not reached the repeat count,
	 *				it applies a rotation each frame. Once the rotation limit is reached, it resets the rotation state.
	 * @param DeltaTime		The time elapsed since the last frame, used for frame-independent movement.
	 */
	void updateRotation(float DeltaTime);

	/**
	 * @brief			Applies a rotation to the tunnelArrow based on the specified direction.
	 * @param rotationDirection	The direction and magnitude of the rotation to apply.
	 */
	void rotateArrow(float rotationDirection);

	/**
	 * @brief			Moves both left and right trigger boxes forward based on the predefined offset.
	 *				The new location is calculated by adding the **triggerBoxOffset** to their current positions.
	 */
	void moveTriggerBoxesForward();

	/**
	 * @brief			Resets the rotation state of the **tunnelArrow**.
	 *				It should be called once the desired rotation is completed or needs to be aborted.
	 */
	void resetRotation();

	/**
	 * @brief			Makes the left and right trigger boxes visible in-game and sets their colors.
	 */
	void showTriggerBoxes();

	/**
	 * @brief			Logs the current status of the tunnel block buffer to the console.
	 */
	void logBufferStatus() const;

	/**
	 * @brief			Logs the memory size used by the tunnelBlocks TArray in bytes.
	 */
	void logTunnelBlocksMemorySize() const;

	/**
	 * **VARIABLE DECLARATIONS**
	 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")			// Always visible, cannot be changed, readable but not writeable in Blueprints, listed in the Trigger category
	UBoxComponent *triggerBoxLeft;								// Represents the left side of the tunnel

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")			// Always visible, cannot be changed, readable but not writeable in Blueprints, listed in the Trigger category
	UBoxComponent *triggerBoxRight;								// Represents the right side of the tunnel

private:
	UPROPERTY(VisibleAnywhere)								// It is always visible in the game editor but cannot be changed
	UArrowComponent *tunnelArrow;								// It represents the direction and skeleton of the tunnel

	FBlockPool *blockPool;									// Block pool
	TArray<ABlock *> tunnelBlocks;								// Blocks in the tunnel
	int32 maxBlocks = 10;									// Maximum number of blocks that can be in the tunnel at the same time

	FVector startPosition = FVector(-250, 0, -250);						// Position of the first block. It represents starting position of the tunnel.
	FVector blockOffset = FVector(0, 800, 0);						// Lenght of block
	FVector triggerBoxOffset = FVector(-800, 0, 0);						// Lenght of block 

	float rotationSpeed = 2.0f;								// Desired rotation speed. It must be divisible bt ninety	--> 1.2
	int32 rotationRepeatCount = 90.0f / rotationSpeed;					// Amount of rotation required
	int32 rotationCount = 0;								// Count of current rotation
	bool isRotatingLeft = false;								// Controls left turn
	bool isRotatingRight = false;								// Controls right turn
	bool isRotationInProgress = false;							// Controls active rotation

	FTimerHandle TurnTimerHandle;
};
