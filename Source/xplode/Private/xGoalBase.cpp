// Fill out your copyright notice in the Description page of Project Settings.


#include "xGoalBase.h"
#include "../xplode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionSystem.h"

// Sets default values
AxGoalBase::AxGoalBase()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USkeletalMeshComponent>("DefaultSceneRoot");
	SetRootComponent(SceneComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("GoalSkeletalMesh");
	/*SkeletalMeshComp->SetCollisionObjectType(XGOALPOSTOBJECT_CHANNEL);*/
	SkeletalMeshComp->SetCollisionProfileName(TEXT("BlockAll"));
	//SkeletalMeshComp->SetSimulatePhysics(true);
	SkeletalMeshComp->SetupAttachment(SceneComp);
	SkeletalMeshComp->SetNotifyRigidBodyCollision(true);

	AIPerceptionStimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuli"));
	AIPerceptionStimuliSourceComp->bAutoRegister = true;
	AIPerceptionStimuliSourceComp->RegisterForSense(TSubclassOf<UAISense_Sight>());
	AIPerceptionStimuliSourceComp->RegisterWithPerceptionSystem();

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	FTransform Transform = UKismetMathLibrary::MakeTransform(FVector(69.f, -41.f, 152.f),FRotator::ZeroRotator, FVector(.25f, 6.f, 3.75f));
	BoxCollisionComp->SetCollisionObjectType(XGOALOBJECT_CHANNEL);
	BoxCollisionComp->SetCollisionProfileName(TEXT("xGoalCollision"));
	/*BoxCollisionComp->SetGenerateOverlapEvents(true);*/
	BoxCollisionComp->SetRelativeTransform(Transform);
	BoxCollisionComp->SetupAttachment(SkeletalMeshComp);
	BoxCollisionComp->SetCanEverAffectNavigation(false);
	BoxCollisionComp->CanCharacterStepUp(false);

	Tags.Add("Goal");
}


// Called when the game starts or when spawned
void AxGoalBase::BeginPlay()
{
	Super::BeginPlay();
	
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}



