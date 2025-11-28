#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CGPlayerController.generated.h"

class UCGChatInput;
class UUserWidget;

UCLASS()
class PROJECT9_API ACGPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACGPlayerController();

    virtual void BeginPlay() override;

    void SetChatMessageString(const FString& InChatMessageString);

    void PrintChatMessageString(const FString& InChatMessageString);

    UFUNCTION(Client, Reliable)
    void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

    UFUNCTION(Server, Reliable)
    void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UCGChatInput> ChatInputWidgetClass;

    UPROPERTY()
    TObjectPtr<UCGChatInput> ChatInputWidgetInstance;

    FString ChatMessageString;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> NotificationTextWidgetClass;

    UPROPERTY()
    TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
    UPROPERTY(Replicated, BlueprintReadOnly)
    FText NotificationText;
};
