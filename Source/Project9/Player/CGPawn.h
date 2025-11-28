#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGPawn.generated.h"

UCLASS()
class PROJECT9_API ACGPawn : public APawn
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;
};
