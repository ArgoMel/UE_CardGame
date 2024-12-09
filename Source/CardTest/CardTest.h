#pragma once
#include "CoreMinimal.h"

#pragma region LoadAsset
/* 오브젝트 에셋*/
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
/* 클래스 에셋 */
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
		const FString functionSignature(__FUNCSIG__);
		UE_LOG(LogTemp, Display, TEXT("Enum function: [%s]"), *functionSignature);
		int32 firstIDX = functionSignature.Find(TEXT("(enum "), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

		if (firstIDX != INDEX_NONE)
		{
			firstIDX += 6;
			const int32 lastIDX = (functionSignature.Len() - 1);

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