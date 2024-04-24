#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include <vector>

class ORIONIX_API FBlockPool
{
	/**
	 * **FUNCTION DECLARATIONS**
	 */
public:
	/**
	 * @brief			Constructor of FBlockPool class
	 */
	FBlockPool();

	/**
	 * @brief			Desturctor of FBlockPool class
	 */
	~FBlockPool();

	/**
	 * @brief			Initializes the block pool with specified parameters and pre-populates it with blocks.
	 *				It ensures the pool is ready with pre-created blocks for use in the game, optimizing runtime performance by avoiding dynamic allocations.
	 *				All mesh paths are defined in meshPahts to create and add blocks to the pool.
	 * @param World			The game world context where the blocks will be spawned.
	 * @param BlockClass		The subclass of ABlock to be used for creating new block instances.
	 */
	void initializePool(UWorld *World, TSubclassOf<ABlock> BlockClass);

	/**
	 * @brief			Removes and returns the last block from the available blocks pool
	 * @return			Last block of the block pool
	 */
	ABlock *popBlock();

	/**
	 * @brief			Removes and returns a random block from the available blocks pool
	 * @return			Random block in the block pool
	 */
	ABlock *getBlockRandomly();

	/**
	 * @brief			Returns a block based on the index of the given parameter
	 * @param index			Index value to be taken from the pool
	 * @return			Corresponding block in the block pool
	 */
	ABlock *getBlockByIndex(int32 index);

	/**
	 * @brief			Returns available block numbers in the block pool
	 * @return			Number of blocks available
	 */
	int32 getPoolSize();

	/**	
	 * @brief			Returns a block to the pool making it available for reuse
	 * @param block			The block to be returned to the pool
	 */
	void returnBlock(ABlock *block);

	/**
	 * @brief			Prints pool status
	 */
	void getPoolStatus() const;

private:
	/**
	 * @brief			Creates a block to add to the pool and adjust its static mesh.
	 * @param meshPath		The path of the static mesh to be set
	 */
	void createBlock(const FString &meshPath);

	/**
	 * @brief			Populates the meshPath list with the paths of the static meshes to use
	 */
	void initializeMeshPath();

	/**
	 * **VARIABLE DECLARATIONS**
	 */
private:
	UWorld *worldContext;					// The game world context where blocks are spawned
	TSubclassOf<ABlock> blockBlueprint;			// Blueprint class for creating new blocks
	TArray<FString> meshPaths;				// Paths to meshes used for block appearances
	TArray<ABlock *> availableBlocks;			// Pool of blocks available for use
};
