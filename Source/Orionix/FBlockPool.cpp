#include "FBlockPool.h"

// Constructor
FBlockPool::FBlockPool(): worldContext(nullptr)
{
}

FBlockPool::~FBlockPool()
{
	for(auto *block : availableBlocks)
	{
		if(block)
		{
			block->Destroy();
		}
	}
}
// Public Functions
void FBlockPool::initializePool(UWorld *World, TSubclassOf<ABlock> BlockClass)
{
	worldContext = World;
	blockBlueprint = BlockClass;

	initializeMeshPath();
	
	for(const FString &meshPath : meshPaths)
	{
		createBlock(meshPath);
	}
}

ABlock *FBlockPool::popBlock()
{
	if(availableBlocks.Num() > 0)
	{
		ABlock *block = availableBlocks.Pop();
		return block;
	}
	return nullptr;
}

ABlock *FBlockPool::getBlockRandomly()
{
	if(availableBlocks.Num() > 0)
	{
		int32 randomIndex = FMath::RandRange(0, availableBlocks.Num() - 1);
		ABlock *block = availableBlocks[randomIndex];
		availableBlocks.RemoveAt(randomIndex);
		return block;
	}
	return nullptr;
}

ABlock *FBlockPool::getBlockByIndex(int32 index)
{
	if(index < availableBlocks.Num())
	{
		return availableBlocks[index];
	}
	return nullptr;
}

int32 FBlockPool::getPoolSize()
{
	return availableBlocks.Num();
}

void FBlockPool::returnBlock(ABlock *block)
{
	if(block)
	{
		availableBlocks.Push(block);
	}
}

void FBlockPool::getPoolStatus() const
{
	UE_LOG(LogTemp, Warning, TEXT("Pool Size: %d, Available: %d"), meshPaths.Num(), availableBlocks.Num());
}

// Private Functions
void FBlockPool::createBlock(const FString &meshPath)
{
	ABlock *NewBlock = worldContext->SpawnActor<ABlock>(blockBlueprint, FVector::ZeroVector, FRotator::ZeroRotator);
	if(NewBlock)
	{
		NewBlock->setBlockMesh(meshPath);
		availableBlocks.Add(NewBlock);
	}
}

void FBlockPool::initializeMeshPath()
{
	// Starting Blocks
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS20_SM.BlockS20_SM'"));

	// Small Blocks
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS1_SM.BlockS1_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS2_SM.BlockS2_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS3_SM.BlockS3_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS4_SM.BlockS4_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS5_SM.BlockS5_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS6_SM.BlockS6_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS7_SM.BlockS7_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS8_SM.BlockS8_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS9_SM.BlockS9_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS10_SM.BlockS10_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS11_SM.BlockS11_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS12_SM.BlockS12_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS13_SM.BlockS13_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS14_SM.BlockS14_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS15_SM.BlockS15_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS16_SM.BlockS16_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS17_SM.BlockS17_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS18_SM.BlockS18_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS19_SM.BlockS19_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS21_SM.BlockS21_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockS/BlockS22_SM.BlockS22_SM'"));

	// Medium Blocks
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM1_SM.BlockM1_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM2_SM.BlockM2_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM3_SM.BlockM3_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM4_SM.BlockM4_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM5_SM.BlockM5_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM6_SM.BlockM6_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM7_SM.BlockM7_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM8_SM.BlockM8_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM9_SM.BlockM9_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM10_SM.BlockM10_SM'"));
	meshPaths.Add(TEXT("StaticMesh'/Game/Blocks/BlockM/BlockM11_SM.BlockM11_SM'"));
}

