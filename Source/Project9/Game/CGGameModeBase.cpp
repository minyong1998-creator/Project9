#include "Game/CGGameModeBase.h"
#include "CGGameStateBase.h"
#include "Player/CGPlayerController.h"
#include "EngineUtils.h"
#include "Player/CGPlayerState.h"

void ACGGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ACGPlayerController* CGPlayerController = Cast<ACGPlayerController>(NewPlayer);
	if (IsValid(CGPlayerController) == true)
	{
		CGPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(CGPlayerController);

		ACGPlayerState* CGPS = CGPlayerController->GetPlayerState<ACGPlayerState>();
		if (IsValid(CGPS) == true)
		{
			CGPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACGGameStateBase* CGGameStateBase = GetGameState<ACGGameStateBase>();
		if (IsValid(CGGameStateBase) == true)
		{
			CGGameStateBase->MulticastRPCBroadcastLoginMessage(CGPS->PlayerNameString);
		}
	}
}

FString ACGGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACGGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACGGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("Secret Number: %s"), *SecretNumberString);
}

void ACGGameModeBase::PrintChatMessageString(ACGPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		for (TActorIterator<ACGPlayerController> It(GetWorld()); It; ++It)
		{
			ACGPlayerController* CGPlayerController = *It;
			if (IsValid(CGPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CGPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ACGPlayerController> It(GetWorld()); It; ++It)
		{
			ACGPlayerController* CGPlayerController = *It;
			if (IsValid(CGPlayerController) == true)
			{
				CGPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ACGGameModeBase::IncreaseGuessCount(ACGPlayerController* InChattingPlayerController)
{
	{
		ACGPlayerState* CGPS = InChattingPlayerController->GetPlayerState<ACGPlayerState>();
		if (IsValid(CGPS) == true)
		{
			CGPS->CurrentGuessCount++;
		}
	}
}

void ACGGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CGPlayerController : AllPlayerControllers)
	{
		ACGPlayerState* CGPS = CGPlayerController->GetPlayerState<ACGPlayerState>();
		if (IsValid(CGPS) == true)
		{
			CGPS->CurrentGuessCount = 0;
		}
	}
}

void ACGGameModeBase::JudgeGame(ACGPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACGPlayerState* CGPS = InChattingPlayerController->GetPlayerState<ACGPlayerState>();
		for (const auto& CGPlayerController : AllPlayerControllers)
		{
			if (IsValid(CGPS) == true)
			{
				FString CombinedMessageString = CGPS->PlayerNameString + TEXT(" has won the game.");
				CGPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CGPlayerController : AllPlayerControllers)
		{
			ACGPlayerState* CGPS = CGPlayerController->GetPlayerState<ACGPlayerState>();
			if (IsValid(CGPS) == true)
			{
				if (CGPS->CurrentGuessCount < CGPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CGPlayerController : AllPlayerControllers)
			{
				CGPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
