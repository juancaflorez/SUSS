﻿#include "SussBrainComponent.h"
#include "SussGameSubsystem.h"
#include "SussTestQueryProviders.h"
#include "SussTestWorldFixture.h"
#if WITH_AUTOMATION_TESTS

UE_DISABLE_OPTIMIZATION


BEGIN_DEFINE_SPEC(FSussBrainTestContextsSpec,
				  "SUSS: Test  Contexts",
				  EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

	TUniquePtr<FSussTestWorldFixture> WorldFixture;
END_DEFINE_SPEC(FSussBrainTestContextsSpec);


void FSussBrainTestContextsSpec::Define()
{
	BeforeEach([this]()
	{
		WorldFixture = MakeUnique<FSussTestWorldFixture>();
		RegisterTestQueryProviders(WorldFixture->GetWorld());
	});
	AfterEach([this]()
	{
		UnregisterTestQueryProviders(WorldFixture->GetWorld());
		WorldFixture.Reset();
	});
	
	Describe("FSussBrainTestContextsSpec", [this]()
	{
		It("Simple single entry, no dimensions", [this]()
		{
			AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(Self->AddComponentByClass(USussBrainComponent::StaticClass(), false, FTransform::Identity, false));

			// Action with no queries
			FSussActionDef Action;
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);

			if (TestEqual("Number of contexts", Contexts.Num(), 1))
			{
				TestEqual("Self reference", Contexts[0].ControlledActor, Self);
				TestNull("Target", Contexts[0].Target.Get());
				TestEqual("Location", Contexts[0].Location, FVector::ZeroVector);
				TestEqual("Rotation", Contexts[0].Rotation, FRotator::ZeroRotator);
			}
		});

		It("One location dimension, single item", [this]()
		{
			AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(Self->AddComponentByClass(USussBrainComponent::StaticClass(), false, FTransform::Identity, false));

			// Query single item
			FSussActionDef Action;
			Action.Queries.Add(FSussQuery { FGameplayTag::RequestGameplayTag(USussTestSingleLocationQueryProvider::TagName) });
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);

			if (TestEqual("Number of contexts", Contexts.Num(), 1))
			{
				TestEqual("Self reference", Contexts[0].ControlledActor, Self);
				TestNull("Target", Contexts[0].Target.Get());
				TestEqual("Location", Contexts[0].Location, FVector(10, -20, 50));
				TestEqual("Rotation", Contexts[0].Rotation, FRotator::ZeroRotator);
			}
		});

		It("One location dimension, multiple items", [this]()
		{
			AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(Self->AddComponentByClass(USussBrainComponent::StaticClass(), false, FTransform::Identity, false));

			// Query multiple locations
			FSussActionDef Action;
			Action.Queries.Add(FSussQuery { FGameplayTag::RequestGameplayTag(USussTestMultipleLocationQueryProvider::TagName) });
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);

			if (TestEqual("Number of contexts", Contexts.Num(), 3))
			{
				TestEqual("Self reference 0", Contexts[0].ControlledActor, Self);
				TestNull("Target 0", Contexts[0].Target.Get());
				TestEqual("Location 0", Contexts[0].Location, FVector(10, -20, 50));
				TestEqual("Rotation 0", Contexts[0].Rotation, FRotator::ZeroRotator);

				TestEqual("Self reference 1", Contexts[1].ControlledActor, Self);
				TestNull("Target 1", Contexts[1].Target.Get());
				TestEqual("Location 1", Contexts[1].Location, FVector(20, 100, -2));
				TestEqual("Rotation 1", Contexts[1].Rotation, FRotator::ZeroRotator);

				TestEqual("Self reference 2", Contexts[2].ControlledActor, Self);
				TestNull("Target 2", Contexts[2].Target.Get());
				TestEqual("Location 2", Contexts[2].Location, FVector(-40, 220, 750));
				TestEqual("Rotation 2", Contexts[2].Rotation, FRotator::ZeroRotator);

			}
		});

		It("Two dimensions, multiple items", [this]()
		{
			AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(Self->AddComponentByClass(USussBrainComponent::StaticClass(), false, FTransform::Identity, false));
			
			// Query multiple locations
			FSussActionDef Action;
			Action.Queries.Add(FSussQuery {FGameplayTag::RequestGameplayTag(USussTestMultipleLocationQueryProvider::TagName) });
			Action.Queries.Add(FSussQuery {FGameplayTag::RequestGameplayTag(USussTestMultipleRotationQueryProvider::TagName) });
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);

			if (TestEqual("Number of contexts", Contexts.Num(), 9))
			{
				TestEqual("Self reference 0", Contexts[0].ControlledActor, Self);
				TestNull("Target 0", Contexts[0].Target.Get());
				TestEqual("Location 0", Contexts[0].Location, FVector(10, -20, 50));
				TestEqual("Rotation 0", Contexts[0].Rotation, FRotator(10, -20, 50));

				TestEqual("Self reference 1", Contexts[1].ControlledActor, Self);
				TestNull("Target 1", Contexts[1].Target.Get());
				TestEqual("Location 1", Contexts[1].Location, FVector(20, 100, -2));
				TestEqual("Rotation 1", Contexts[1].Rotation, FRotator(10, -20, 50));

				TestEqual("Self reference 2", Contexts[2].ControlledActor, Self);
				TestNull("Target 2", Contexts[2].Target.Get());
				TestEqual("Location 2", Contexts[2].Location, FVector(-40, 220, 750));
				TestEqual("Rotation 2", Contexts[2].Rotation, FRotator(10, -20, 50));

				TestEqual("Self reference3", Contexts[3].ControlledActor, Self);
				TestNull("Target3", Contexts[3].Target.Get());
				TestEqual("Location3", Contexts[3].Location, FVector(10, -20, 50));
				TestEqual("Rotation3", Contexts[3].Rotation, FRotator(20, 100, -2));

				TestEqual("Self reference 4", Contexts[4].ControlledActor, Self);
				TestNull("Target 4", Contexts[4].Target.Get());
				TestEqual("Location 4", Contexts[4].Location, FVector(20, 100, -2));
				TestEqual("Rotation 4", Contexts[4].Rotation, FRotator(20, 100, -2));

				TestEqual("Self reference 5", Contexts[5].ControlledActor, Self);
				TestNull("Target 5", Contexts[5].Target.Get());
				TestEqual("Location 5", Contexts[5].Location, FVector(-40, 220, 750));
				TestEqual("Rotation 5", Contexts[5].Rotation, FRotator(20, 100, -2));
	
				TestEqual("Self reference 6", Contexts[6].ControlledActor, Self);
				TestNull("Target 6", Contexts[6].Target.Get());
				TestEqual("Location 6", Contexts[6].Location, FVector(10, -20, 50));
				TestEqual("Rotation 6", Contexts[6].Rotation, FRotator(-40, 220, 750));

				TestEqual("Self reference 7", Contexts[7].ControlledActor, Self);
				TestNull("Target 7", Contexts[7].Target.Get());
				TestEqual("Location 7", Contexts[7].Location, FVector(20, 100, -2));
				TestEqual("Rotation 7", Contexts[7].Rotation, FRotator(-40, 220, 750));

				TestEqual("Self reference 8", Contexts[8].ControlledActor, Self);
				TestNull("Target 8", Contexts[8].Target.Get());
				TestEqual("Location 8", Contexts[8].Location, FVector(-40, 220, 750));
				TestEqual("Rotation 8", Contexts[8].Rotation, FRotator(-40, 220, 750));
			}
		});

		It("Named params combined with locations", [this]()
		{
			AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(
				Self->AddComponentByClass(USussBrainComponent::StaticClass(), false, FTransform::Identity, false));

			FSussActionDef Action;
			Action.Queries.Add(FSussQuery {FGameplayTag::RequestGameplayTag(USussTestNamedFloatValueQueryProvider::TagName) }); // 2 items
			Action.Queries.Add(FSussQuery {FGameplayTag::RequestGameplayTag(USussTestMultipleLocationQueryProvider::TagName) }); // 3 items
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);

			if (TestEqual("Number of contexts", Contexts.Num(), 6))
			{
				TestEqual("Self reference 0", Contexts[0].ControlledActor, Self);
				TestEqual("Location 0", Contexts[0].Location, FVector(10, -20, 50));
				if (TestTrue("Named Range 0",Contexts[0].NamedValues.Contains("Range")))
				{
					TestEqual("Named 0", Contexts[0].NamedValues["Range"].Value.Get<float>(), 2000.0f);
				}

				TestEqual("Self reference 1", Contexts[1].ControlledActor, Self);
				TestEqual("Location 1", Contexts[1].Location, FVector(10, -20, 50));
				if (TestTrue("Named Range 1",Contexts[1].NamedValues.Contains("Range")))
				{
					TestEqual("Named 1", Contexts[1].NamedValues["Range"].Value.Get<float>(), 5000.0f);
				}

				TestEqual("Self reference 2", Contexts[2].ControlledActor, Self);
				TestEqual("Location 2", Contexts[2].Location, FVector(20, 100, -2));
				if (TestTrue("Named Range 2",Contexts[2].NamedValues.Contains("Range")))
				{
					TestEqual("Named 2", Contexts[2].NamedValues["Range"].Value.Get<float>(), 2000.0f);
				}

				TestEqual("Self reference 3", Contexts[3].ControlledActor, Self);
				TestEqual("Location 3", Contexts[3].Location, FVector(20, 100, -2));
				if (TestTrue("Named Range 3",Contexts[3].NamedValues.Contains("Range")))
				{
					TestEqual("Named 3", Contexts[3].NamedValues["Range"].Value.Get<float>(), 5000.0f);
				}

				TestEqual("Self reference 4", Contexts[4].ControlledActor, Self);
				TestEqual("Location 4", Contexts[4].Location, FVector(-40, 220, 750));
				if (TestTrue("Named Range 4",Contexts[4].NamedValues.Contains("Range")))
				{
					TestEqual("Named 4", Contexts[4].NamedValues["Range"].Value.Get<float>(), 2000.0f);
				}

				TestEqual("Self reference 5", Contexts[5].ControlledActor, Self);
				TestEqual("Location 5", Contexts[5].Location, FVector(-40, 220, 750));
				if (TestTrue("Named Range 5",Contexts[5].NamedValues.Contains("Range")))
				{
					TestEqual("Named 5", Contexts[5].NamedValues["Range"].Value.Get<float>(), 5000.0f);
				}
			}
		});

		It("Named params x2",
		   [this]()
		{
			AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(
				Self->AddComponentByClass(USussBrainComponent::StaticClass(),
				                          false,
				                          FTransform::Identity,
				                          false));

			FSussActionDef Action;
			Action.Queries.Add(FSussQuery{
				FGameplayTag::RequestGameplayTag(USussTestNamedFloatValueQueryProvider::TagName)
			}); // 2 items
			Action.Queries.Add(FSussQuery{
				FGameplayTag::RequestGameplayTag(USussTestNamedLocationValueQueryProvider::TagName)
			}); // 3 items
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);

			if (TestEqual("Number of contexts", Contexts.Num(), 6))
			{
				TestEqual("Self reference 0", Contexts[0].ControlledActor, Self);
				if (TestTrue("Named MapRef 0", Contexts[0].NamedValues.Contains("MapRef")))
				{
					TestEqual("Named MapRef 0",
					          Contexts[0].NamedValues["MapRef"].Value.Get<FVector>(),
					          FVector(120, -450, 80));
				}
				if (TestTrue("Named Range 0", Contexts[0].NamedValues.Contains("Range")))
				{
					TestEqual("Named Range 0", Contexts[0].NamedValues["Range"].Value.Get<float>(), 2000.0f);
				}

				TestEqual("Self reference 1", Contexts[1].ControlledActor, Self);
				if (TestTrue("Named MapRef 1", Contexts[1].NamedValues.Contains("MapRef")))
				{
					TestEqual("Named MapRef 1",
					          Contexts[1].NamedValues["MapRef"].Value.Get<FVector>(),
					          FVector(120, -450, 80));
				}
				if (TestTrue("Named Range 1", Contexts[1].NamedValues.Contains("Range")))
				{
					TestEqual("Named 1", Contexts[1].NamedValues["Range"].Value.Get<float>(), 5000.0f);
				}

				TestEqual("Self reference 2", Contexts[2].ControlledActor, Self);
				if (TestTrue("Named MapRef 2", Contexts[2].NamedValues.Contains("MapRef")))
				{
					TestEqual("Named MapRef 2",
					          Contexts[2].NamedValues["MapRef"].Value.Get<FVector>(),
					          FVector(70, 123, -210));
				}
				if (TestTrue("Named Range 2", Contexts[2].NamedValues.Contains("Range")))
				{
					TestEqual("Named 2", Contexts[2].NamedValues["Range"].Value.Get<float>(), 2000.0f);
				}

				TestEqual("Self reference 3", Contexts[3].ControlledActor, Self);
				if (TestTrue("Named MapRef 3", Contexts[3].NamedValues.Contains("MapRef")))
				{
					TestEqual("Named MapRef 3",
					          Contexts[3].NamedValues["MapRef"].Value.Get<FVector>(),
					          FVector(70, 123, -210));
				}
				if (TestTrue("Named Range 3", Contexts[3].NamedValues.Contains("Range")))
				{
					TestEqual("Named 3", Contexts[3].NamedValues["Range"].Value.Get<float>(), 5000.0f);
				}

				TestEqual("Self reference 4", Contexts[4].ControlledActor, Self);
				if (TestTrue("Named MapRef 4", Contexts[4].NamedValues.Contains("MapRef")))
				{
					TestEqual("Named MapRef 4",
					          Contexts[4].NamedValues["MapRef"].Value.Get<FVector>(),
					          FVector(-35, 65, 0));
				}
				if (TestTrue("Named Range 4", Contexts[4].NamedValues.Contains("Range")))
				{
					TestEqual("Named 4", Contexts[4].NamedValues["Range"].Value.Get<float>(), 2000.0f);
				}

				TestEqual("Self reference 5", Contexts[5].ControlledActor, Self);
				if (TestTrue("Named MapRef 5", Contexts[5].NamedValues.Contains("MapRef")))
				{
					TestEqual("Named MapRef 5",
					          Contexts[5].NamedValues["MapRef"].Value.Get<FVector>(),
					          FVector(-35, 65, 0));
				}
				if (TestTrue("Named Range 5", Contexts[5].NamedValues.Contains("Range")))
				{
					TestEqual("Named 5", Contexts[5].NamedValues["Range"].Value.Get<float>(), 5000.0f);
				}
			}
		});
		

		It("Query caching works as intended", [this]()
		{
		   	AActor* Self = WorldFixture->GetWorld()->SpawnActor<AActor>();
			auto Brain = Cast<USussBrainComponent>(Self->AddComponentByClass(USussBrainComponent::StaticClass(), false, FTransform::Identity, false));
			USussTestSingleLocationQueryProvider* Q = USussTestSingleLocationQueryProvider::StaticClass()->GetDefaultObject<USussTestSingleLocationQueryProvider>();

			// reset this manually, query objects are reused
			Q->NumTimesRun = 0;

			// Query single item
			FSussActionDef Action;
			Action.Queries.Add(FSussQuery {FGameplayTag::RequestGameplayTag(USussTestSingleLocationQueryProvider::TagName) });
			TArray<FSussContext> Contexts;
			Brain->GenerateContexts(Self, Action, Contexts);
			TestEqual("Query run count", Q->NumTimesRun, 1);
			
			if (TestEqual("Number of contexts", Contexts.Num(), 1))
			{
				TestEqual("Self reference", Contexts[0].ControlledActor, Self);
				TestNull("Target", Contexts[0].Target.Get());
				TestEqual("Location", Contexts[0].Location, FVector(10, -20, 50));
				TestEqual("Rotation", Contexts[0].Rotation, FRotator::ZeroRotator);
			}
		    // Now repeat; result should be the same because no time has passed & params are the same
		    Contexts.Empty();
		    Brain->GenerateContexts(Self, Action, Contexts);
			// Should re-use results
			TestEqual("Query should have re-used results", Q->NumTimesRun, 1);

			// Now make some time pass, above the 0.5s default query timeout & return 10x data value
			// Queries are ticked through the suss subsystem
			GetSUSS(WorldFixture->GetWorld())->Tick(1);
			// Tick the brain for consistency
			Brain->TickComponent(1, LEVELTICK_All, nullptr);

			// Running the query again should make it go down the other path
			Contexts.Empty();
			Brain->GenerateContexts(Self, Action, Contexts);
			TestEqual("Query should have run again because of time", Q->NumTimesRun, 2);

			// Issue a query with different params
			Action.Queries[0].Params.Add("OverrideX", FSussParameter(30.0f));
			Action.Queries[0].Params.Add("OverrideY", FSussParameter(-40.0f));
			Contexts.Empty();
			Brain->GenerateContexts(Self, Action, Contexts);
			TestEqual("Query should have run again because of parameters", Q->NumTimesRun, 3);

			Contexts.Empty();
			Brain->GenerateContexts(Self, Action, Contexts);
			TestEqual("Query should have re-used", Q->NumTimesRun, 3);

			// Going back to empty params should re-use the older query
			Action.Queries[0].Params.Empty();
			Contexts.Empty();
			Brain->GenerateContexts(Self, Action, Contexts);
			TestEqual("Query should have re-used older query", Q->NumTimesRun, 3);

			// Test going back to new query again
			Action.Queries[0].Params.Add("OverrideX", FSussParameter(30.0f));
			Action.Queries[0].Params.Add("OverrideY", FSussParameter(-40.0f));
			Contexts.Empty();
			Brain->GenerateContexts(Self, Action, Contexts);
			TestEqual("Query should have been re-used, newer again", Q->NumTimesRun, 3);
			
			// Changing which "self" we refer to should invalidate, if relevant
			AActor* WrongSelf = WorldFixture->GetWorld()->SpawnActor<AActor>();

			Contexts.Empty();
			Brain->GenerateContexts(WrongSelf, Action, Contexts);
			TestEqual("Query should run, changed self", Q->NumTimesRun, 4);
			
		});

	});
}

UE_ENABLE_OPTIMIZATION

#endif
