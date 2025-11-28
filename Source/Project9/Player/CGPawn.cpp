#include "CGPawn.h"
#include "Project9.h"


void ACGPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = Project9FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *Project9FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Project9FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ACGPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = Project9FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *Project9FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Project9FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
