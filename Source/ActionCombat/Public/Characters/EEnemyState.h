// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Range UMETA(DisplayName = "Range"),
	Charge UMETA(DisplayName = "Charge"),
	Melee UMETA(DisplayName = "Melee")
};