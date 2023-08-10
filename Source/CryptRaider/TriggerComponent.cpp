// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Display, TEXT("TriggerComponent created!"));
}

