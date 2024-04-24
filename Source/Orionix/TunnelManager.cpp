#include "TunnelManager.h"
#include "TimerManager.h"

// Constructor
ATunnelManager::ATunnelManager()
{
	PrimaryActorTick.bCanEverTick = true;
	tunnelArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterLine"));
	generateTriggerBoxPairs();
	blockPool = new FBlockPool();
}

// Destructor
ATunnelManager::~ATunnelManager()
{
	delete blockPool;
	blockPool = nullptr;
}

// Protected Functions
void ATunnelManager::BeginPlay()
{
	Super::BeginPlay();
	FRotator initialRotation = FRotator(0.f, 90.f, 0.f);
	tunnelArrow->SetWorldRotation(initialRotation);								// Set arrow component rotation
	blockPool->initializePool(GetWorld(), ABlock::StaticClass());				// Pool created and filled
	subscribeToBlockEvents();													// Subscribed to trigger events of block
	initializeTunnel();															// Tunnel initialized
	
	//triggerRandomTurn();
}

void ATunnelManager::OnTriggerBoxOverlapLeft(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(!isRotationInProgress)
	{
		isRotatingLeft = true;
		isRotationInProgress = true;
	}
}

void ATunnelManager::OnTriggerBoxOverlapRight(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(!isRotationInProgress)
	{
		isRotatingRight = true;
		isRotationInProgress = true;
	}
}

// Public Functions
void ATunnelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updateRotation(DeltaTime);
}

void ATunnelManager::initializeTunnel()
{
	for(int32 i = 0; i < maxBlocks; i++)
	{
		ABlock *newBlock = blockPool->getBlockRandomly();
		if(!newBlock) return;
		addBlockToBuffer(newBlock);
		newBlock->rotateBlockRandomly();
		newBlock->AttachToComponent(tunnelArrow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		newBlock->setBlockLocation(startPosition);
		startPosition += blockOffset;
		newBlock->unhideAndEnableCollision();
	}
}

void ATunnelManager::subscribeToBlockEvents()
{
	for(int32 i = 0; i < blockPool->getPoolSize(); i++)
	{
		ABlock *block = blockPool->getBlockByIndex(i);
		if(block != nullptr)
		{
			block->onBlockTriggered.AddDynamic(this, &ATunnelManager::handleBlockTrigger);
		}
	}
}

void ATunnelManager::addBlockToTunnel()
{
	ABlock *newBlock = blockPool->getBlockRandomly();
	if(!newBlock) return;

	newBlock->rotateBlockRandomly();
	newBlock->AttachToComponent(tunnelArrow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ABlock *lastBlock = tunnelBlocks.Last();
	FVector lastestBlockPosition = lastBlock->GetRootComponent()->GetRelativeLocation();
	FVector newLocation = lastestBlockPosition + blockOffset;
	newBlock->setBlockLocation(newLocation);
	addBlockToBuffer(newBlock);
	newBlock->unhideAndEnableCollision();
}

void ATunnelManager::removeBlockFromTunnel()
{
	ABlock *oldestBlock = tunnelBlocks[0];
	if(oldestBlock != nullptr)
	{
		oldestBlock->hideAndDisableCollision();
		oldestBlock->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		oldestBlock->flag = false;
		blockPool->returnBlock(oldestBlock);
	}
	removeBlockFromBuffer();
}

void ATunnelManager::handleBlockTrigger()
{
	addBlockToTunnel();
	removeBlockFromTunnel();
	moveTriggerBoxesForward();
}

void ATunnelManager::turnLeft()
{
	if(!isRotationInProgress)
	{
		isRotatingLeft = true;
		isRotationInProgress = true;
	}
}

void ATunnelManager::turnRight()
{
	if(!isRotationInProgress)
	{
		isRotatingRight = true;
		isRotationInProgress = true;
	}
}

void ATunnelManager::triggerRandomTurn()
{
	float minDelay = 4.0f;
	float maxDelay = 7.0f;

	float randomDelay = FMath::RandRange(minDelay, maxDelay);
	GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &ATunnelManager::onTurnTimerExpired, randomDelay, false);

	int32 randomDirection = FMath::RandRange(0, 1);
	if(randomDirection == 0)
	{
		turnLeft();
	}
	else
	{
		turnRight();
	}
}

void ATunnelManager::onTurnTimerExpired()
{
	triggerRandomTurn();
}

// Private Functions
void ATunnelManager::generateTriggerBoxPairs()
{
	triggerBoxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxLeft"));
	triggerBoxRight = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxRight"));

	triggerBoxLeft->SetupAttachment(RootComponent);
	triggerBoxLeft->SetBoxExtent(FVector(10.0f, 2400.0f, 250.0f));
	triggerBoxLeft->SetRelativeLocation(FVector(-1600.0f, 250.0f, 500.0f));
	triggerBoxLeft->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	triggerBoxLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerBoxLeft->SetCollisionResponseToAllChannels(ECR_Ignore);
	triggerBoxLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//triggerBoxLeft->OnComponentBeginOverlap.AddDynamic(this, &ATunnelManager::OnTriggerBoxOverlapLeft);

	triggerBoxRight->SetupAttachment(RootComponent);
	triggerBoxRight->SetBoxExtent(FVector(10.0f, 2400.0f, 250.0f));
	triggerBoxRight->SetRelativeLocation(FVector(-1600.0f, -250.0f, 500.0f));
	triggerBoxRight->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	triggerBoxRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerBoxRight->SetCollisionResponseToAllChannels(ECR_Ignore);
	triggerBoxRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//triggerBoxRight->OnComponentBeginOverlap.AddDynamic(this, &ATunnelManager::OnTriggerBoxOverlapRight);
}

void ATunnelManager::addBlockToBuffer(ABlock *newBlock)
{
	tunnelBlocks.Add(newBlock);
}

void ATunnelManager::removeBlockFromBuffer()
{
	tunnelBlocks.RemoveAt(0);
}

void ATunnelManager::updateRotation(float DeltaTime)
{
	if((isRotatingLeft || isRotatingRight) && (rotationCount < rotationRepeatCount))
	{
		float rotationDirection = isRotatingRight ? rotationSpeed : -rotationSpeed;
		rotateArrow(rotationDirection);
	}
	else
	{
		resetRotation();
	}
}

void ATunnelManager::rotateArrow(float rotationDirection)
{
	FRotator rotationAngle = FRotator(rotationDirection, 0.0f, 0.0f);
	tunnelArrow->AddLocalRotation(rotationAngle);
	rotationCount++;
}

void ATunnelManager::moveTriggerBoxesForward()
{
	FVector TBL_Location = triggerBoxLeft->GetRelativeLocation();
	FVector TBR_Location = triggerBoxRight->GetRelativeLocation();

	TBL_Location += triggerBoxOffset;
	TBR_Location += triggerBoxOffset;
	triggerBoxLeft->SetRelativeLocation(TBL_Location);
	triggerBoxRight->SetRelativeLocation(TBR_Location);
}

void ATunnelManager::resetRotation()
{
	isRotatingLeft = false;
	isRotatingRight = false;
	isRotationInProgress = false;
	rotationCount = 0;
}

void ATunnelManager::showTriggerBoxes()
{
	if(triggerBoxLeft)
	{
		triggerBoxLeft->SetHiddenInGame(false);
		triggerBoxLeft->ShapeColor = FColor::Green;
	}

	if(triggerBoxRight)
	{
		triggerBoxRight->SetHiddenInGame(false);
		triggerBoxRight->ShapeColor = FColor::Red;
	}
}

void ATunnelManager::logBufferStatus() const
{
	UE_LOG(LogTemp, Warning, TEXT("Total Blocks: %d"), tunnelBlocks.Num());
}

void ATunnelManager::logTunnelBlocksMemorySize() const
{
	int32 NumberOfElements = tunnelBlocks.Num();
	int32 PointerSize = sizeof(ABlock *);
	int32 ArrayMemoryUsage = NumberOfElements * PointerSize;
	UE_LOG(LogTemp, Log, TEXT("TArray is using approximately %d bytes for its elements."), ArrayMemoryUsage);
}

