﻿#include "SussQueryProvider.h"


uint32 USussQueryProvider::HashQueryRequest(AActor* Self, const TMap<FName, FSussParameter>& Params)
{
	uint32 Hash = 0;

	if (bSelfIsRelevant)
		Hash = GetTypeHash(Self);
	
	for (const auto& Pair : Params)
	{
		Hash = HashCombine(Hash, GetTypeHash(Pair.Key));
		Hash = HashCombine(Hash, GetTypeHash(Pair.Value));
	}
	return Hash;
}

bool USussQueryProvider::ParamsMatch(const TMap<FName, FSussParameter>& Params1,
                                     const TMap<FName, FSussParameter>& Params2) const
{
	if (Params1.Num() != Params2.Num())
		return false;
	
	for (auto ParamEntry: Params1)
	{
		if (auto pParam2 = Params2.Find(ParamEntry.Key))
		{
			if (ParamEntry.Value != *pParam2)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void USussTargetQueryProvider::ExecuteQuery(USussBrainComponent* Brain,
	AActor* Self,
	const TMap<FName, FSussParameter>& Params,
	TArray<TWeakObjectPtr<AActor>>& OutResults)
{
	// Subclasses can override; this one needs to proxy to BP-compatible (non-weak pointer) version
	TArray<AActor*> BPArray;
	ExecuteQueryBP(Brain, Self, Params, BPArray);
	OutResults.Append(BPArray);
}

void USussLocationQueryProvider::ExecuteQuery(USussBrainComponent* Brain,
	AActor* Self,
	const TMap<FName, FSussParameter>& Params,
	TArray<FVector>& OutResults)
{
	// Subclasses can override this, call BP version by default
	ExecuteQueryBP(Brain, Self, Params, OutResults);
}

void USussNamedValueQueryProvider::AddValueStruct(FSussContextValueStructBase* Struct)
{
	const FSussContextValue S(MakeShareable(Struct));
	GetTempArray().Add(S);
}

void USussNamedValueQueryProvider::ExecuteQuery(USussBrainComponent* Brain,
                                                AActor* Self,
                                                const TMap<FName, FSussParameter>& Params,
                                                TArray<FSussContextValue>& OutResults)
{

	// Subclasses can override this, call BP version  by default
	// Array is not passed because it's BP-incompatible, use utility methods AddNamedValue.. 
	ExecuteQueryBP(Brain, Self, Params);
}

