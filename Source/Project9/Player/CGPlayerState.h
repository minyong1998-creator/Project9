#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CGPlayerState.generated.h"

UCLASS()
class PROJECT9_API ACGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACGPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
};
