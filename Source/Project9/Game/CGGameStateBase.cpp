#include "CGGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CGPlayerController.h"

void ACGGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACGPlayerController* CGPC = Cast<ACGPlayerController>(PC);
			if (IsValid(CGPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CGPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
