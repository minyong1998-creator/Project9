#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGGameModeBase.generated.h"

class ACGPlayerController;

UCLASS()
class PROJECT9_API ACGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(ACGPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ACGPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ACGPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACGPlayerController>> AllPlayerControllers;
};
