#pragma once
#include "Engine.h"
#include "EnhancedInputComponent.h"
#include "AudioDevice.h"

#pragma region LoadAsset
/*    Object Asset을 받습니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 오브젝트의 클래스
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
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
}\
/*    Class Asset을 받습니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 클래스 타입
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
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
}\

#pragma endregion

template<typename T>
FString EnumToFString(T enumValue)
{
#if !WITH_EDITOR
	return "";
#else
	static FString enumName = TEXT("/Script/VNModule.");
	static bool bEnumNameInit = false;
	// Init it once
	if (bEnumNameInit == false)
	{
		bEnumNameInit = true;
		FString functionSignature(__FUNCSIG__);
		UE_LOG(LogTemp, Display, TEXT("Enum funcsig: [%s]"), *functionSignature);
		int32 firstIDX = functionSignature.Find(TEXT("(enum "), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

		if (firstIDX != INDEX_NONE)
		{
			firstIDX += 6;
			int32 lastIDX = (functionSignature.Len() - 1);

			if (functionSignature.IsValidIndex(firstIDX) &&
				functionSignature.IsValidIndex(lastIDX) && firstIDX < lastIDX)
			{
				enumName.Append(functionSignature.Mid(firstIDX, lastIDX - firstIDX));
				UE_LOG(LogTemp, Display, TEXT("Enum name: [%s]"), *enumName);
			}
		}
	}

	if (enumName.IsEmpty())
	{
		return FString("ENUM_Invalid");
	}
	const UEnum* enumPtr = FindObject<UEnum>(nullptr, *enumName, true);
	if (!enumPtr)
	{
		return FString("ENUM_Invalid");
	}
	return enumPtr->GetDisplayNameTextByIndex(static_cast<int32>(enumValue)).ToString();
#endif
}