// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent * PhysicHandle = GetPhysicsHandle();
	if (PhysicHandle && PhysicHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent * PhysicHandle = GetPhysicsHandle();
	UPrimitiveComponent* GrabbedComponent = PhysicHandle->GetGrabbedComponent();
	if (PhysicHandle && GrabbedComponent)
	{
		AActor* GrabbedActor = GrabbedComponent->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicHandle->ReleaseComponent();
	}
	
}

void UGrabber::Grab(){
	
	UPhysicsHandleComponent * PhysicHandle = GetPhysicsHandle();
	FHitResult HitResult;

	if (PhysicHandle && GetGrabbableInReach(HitResult))
	{
		UPrimitiveComponent * ComponentHit = HitResult.GetComponent();
		ComponentHit->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ComponentHit->SetSimulatePhysics(true);
		PhysicHandle->GrabComponentAtLocationWithRotation
		(
   			ComponentHit,
    		NAME_None,
    		HitResult.ImpactPoint,
    		GetComponentRotation()
		);
	}

}

UPhysicsHandleComponent * UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent * PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle component found!"));	
	}
	return PhysicsHandle;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;	

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(Radius);
	
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionSphere
	);
}