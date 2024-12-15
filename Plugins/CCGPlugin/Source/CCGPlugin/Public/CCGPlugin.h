// Copyright Epic Games, Inc. All Rights Reserved.

// ReSharper disable CppUnusedIncludeDirective
#pragma once
#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Common/CCGConstVar.h"
#include "Modules/ModuleManager.h"

#pragma region LOG_MESSAGE
#define LOG_CLONE_CARD(CARDNAME,ABILITY_INT)\
FString::printf(TEXT("%s cloned up to + %d times!"),*CARDNAME.ToString(),ABILITY_INT);

#define LOG_PICKUP_CARD(ABILITY_INT)\
FString::printf(TEXT("Player picked up + %d cards"),ABILITY_INT);

#define LOG_INCREASE_ATTACK(CARDNAME,ABILITY_INT)\
FString::printf(TEXT("%s gained + %d Attack"),*CARDNAME.ToString(),ABILITY_INT);
#pragma endregion

#pragma region NullCheck
#define IF_RET_VOID(RETURN_POINTER)\
if(!RETURN_POINTER)\
{\
UE_LOG(LogTemp, Warning, TEXT("%hs 유효하지 않음"), #RETURN_POINTER);\
return;\
}

#define IF_RET_BOOL(RETURN_POINTER)\
if(!RETURN_POINTER)\
{\
UE_LOG(LogTemp, Warning, TEXT("%hs 유효하지 않음"), #RETURN_POINTER);\
return false;\
}

#define IF_RET_NULL(RETURN_POINTER)\
if(!RETURN_POINTER)\
{\
UE_LOG(LogTemp, Warning, TEXT("%hs 유효하지 않음"), #RETURN_POINTER);\
return nullptr;\
}

#define IF_RET(RETURN_TYPE,RETURN_POINTER)\
if(!RETURN_POINTER)\
{\
UE_LOG(LogTemp, Warning, TEXT("%hs 유효하지 않음"), #RETURN_POINTER);\
return RETURN_TYPE();\
}
#pragma endregion

#pragma region LoadAsset
//오브젝트 에셋
#define GetObjectAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) \
{\
    RETURN_POINTER = __##RETURN_POINTER.Object;\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed GetObjectAsset : %d"),TEXT(PATH));\
	RETURN_POINTER = nullptr;\
}

#define AddObjectAsset(RETURN_POINTER,INDEX, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> RETURN_POINTER##NAME(TEXT(PATH));\
if (RETURN_POINTER##INDEX.Succeeded()) \
{\
    RETURN_POINTER.Add(RETURN_POINTER##INDEX.Object);\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed AddObjectAsset : %d"),TEXT(PATH));\
}

//클래스 에셋
#define GetClassAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FClassFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) \
{\
    RETURN_POINTER = __##RETURN_POINTER.Class;\
}\
else \
{\
	UE_LOG(LogTemp,Warning,TEXT("Failed GetClassAsset : %d"),TEXT(PATH));\
	RETURN_POINTER = nullptr;\
}
#pragma endregion

class FCCGPluginModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
