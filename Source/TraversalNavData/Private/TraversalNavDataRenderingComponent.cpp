//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataRenderingComponent.h"

#include "Engine/Engine.h"
#include "TraversalNavData.h"
#include "TraversalNavDataFunctionLibrary.h"
#include "TraversalNavDataSettings.h"
#include "SceneManagement.h"
#include "SceneView.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavDataRenderingComponent)

namespace FTraversalNavDataRenderingHelpers
{
	static constexpr float DefaultLineThickness = 0.0f;
	static constexpr float LineThickness = 5.f;
	static constexpr float LineDirectionThickness = 2.f;

	bool LineInView(const FVector& Start, const FVector& End, const FSceneView* View, float DrawDistanceSq)
	{
		if (FVector::DistSquaredXY(Start, View->ViewMatrices.GetViewOrigin()) > DrawDistanceSq ||
			FVector::DistSquaredXY(End, View->ViewMatrices.GetViewOrigin()) > DrawDistanceSq)
		{
			return false;
		}

		for (int32 PlaneIdx = 0; PlaneIdx < View->ViewFrustum.Planes.Num(); ++PlaneIdx)
		{
			const FPlane& CurPlane = View->ViewFrustum.Planes[PlaneIdx];
			if (CurPlane.PlaneDot(Start) > 0.f && CurPlane.PlaneDot(End) > 0.f)
			{
				return false;
			}
		}

		return true;
	}

	bool PointInView(const FVector& Position, const FSceneView* View, float DrawDistanceSq)
	{
		if (FVector::DistSquaredXY(Position, View->ViewMatrices.GetViewOrigin()) > DrawDistanceSq)
		{
			return false;
		}

		for (int32 PlaneIdx = 0; PlaneIdx < View->ViewFrustum.Planes.Num(); ++PlaneIdx)
		{
			const FPlane& CurPlane = View->ViewFrustum.Planes[PlaneIdx];
			if (CurPlane.PlaneDot(Position) > 0.f)
			{
				return false;
			}
		}

		return true;
	}

	bool LineInCorrectDistance(const FVector& Start, const FVector& End, const FSceneView* View, float DrawDistanceSq, FVector::FReal CorrectDistance = -1.)
	{
		const FVector::FReal MaxDistanceSq = (CorrectDistance > 0.) ? FMath::Square(CorrectDistance) : DrawDistanceSq;
		return	FVector::DistSquaredXY(Start, View->ViewMatrices.GetViewOrigin()) < MaxDistanceSq &&
			FVector::DistSquaredXY(End, View->ViewMatrices.GetViewOrigin()) < MaxDistanceSq;
	}

}
// FTraversalNavSegmentDataRenderSceneProxy -------------------------------------------------------------------------------------------------------------

class FTraversalNavSegmentDataRenderSceneProxy : public FPrimitiveSceneProxy
{
public:
	FTraversalNavSegmentDataRenderSceneProxy(const class UTraversalNavDataRenderingComponent& InRenderComp);

	// Begin FPrimitiveSceneProxy
	virtual SIZE_T GetTypeHash() const override;
	virtual uint32 GetMemoryFootprint(void) const override;
	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
	uint32 GetAllocatedSize(void) const { return FPrimitiveSceneProxy::GetAllocatedSize(); }
	virtual bool CanBeOccluded() const override;
	// End FPrimitiveSceneProxy


private:
	TArray<FTraversalNavSegmentData> TraversalNavSegments;
};

FTraversalNavSegmentDataRenderSceneProxy::FTraversalNavSegmentDataRenderSceneProxy(const UTraversalNavDataRenderingComponent& InRenderComp)
	: FPrimitiveSceneProxy(&InRenderComp)
{
	ATraversalNavData* TraversalNavData = Cast<ATraversalNavData>(InRenderComp.GetOwner());
	if (TraversalNavData == nullptr)
	{
		return;
	}
	TraversalNavSegments = TraversalNavData ->GetTraversalNavSegments();
}

SIZE_T FTraversalNavSegmentDataRenderSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}

uint32 FTraversalNavSegmentDataRenderSceneProxy::GetMemoryFootprint(void) const
{
	return sizeof * this + GetAllocatedSize();
}

void FTraversalNavSegmentDataRenderSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>();
	if (TraversalNavDataSettings == nullptr)
	{
		return;
	}

	const float MaxDrawDistanceSqr = FMath::Square(TraversalNavDataSettings->MaxDrawDistance);
	const float LineDirectionLength = TraversalNavDataSettings->VoxelizationCellSize + 10.0f;
	const float DrawOffset = TraversalNavDataSettings->DrawOffset;

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		if (VisibilityMap & (1 << ViewIndex))
		{
			const FSceneView* View = Views[ViewIndex];
			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);

			for (const FTraversalNavSegmentData& Segment : TraversalNavSegments)
			{
				if (FTraversalNavDataRenderingHelpers::LineInView(Segment.Start, Segment.End, View, MaxDrawDistanceSqr))
				{
					const FColor LineColor = TraversalNavDataSettings->GetTraversalNavSegmentColor(Segment);
					const FVector Normal = Segment.GetNormal();
					const FVector MidPoint = (Segment.Start + Segment.End) * 0.5f;
					const FVector Offset = Normal * DrawOffset;

					if (FTraversalNavDataRenderingHelpers::LineInCorrectDistance(Segment.Start, Segment.End, View, MaxDrawDistanceSqr))
					{
						PDI->DrawLine(Segment.Start + Offset, Segment.End + Offset, LineColor, SDPG_World, FTraversalNavDataRenderingHelpers::LineThickness, 0, true);
						PDI->DrawLine(MidPoint + Offset, MidPoint + Segment.Binormal * LineDirectionLength + Offset, LineColor, SDPG_World, FTraversalNavDataRenderingHelpers::LineDirectionThickness, 0, true);
						PDI->DrawLine(MidPoint + Offset, MidPoint + Normal * LineDirectionLength + Offset, LineColor, SDPG_World, FTraversalNavDataRenderingHelpers::LineDirectionThickness, 0, true);
					}
					else
					{
						PDI->DrawLine(Segment.Start + Offset, Segment.End + Offset, LineColor, SDPG_World, FTraversalNavDataRenderingHelpers::DefaultLineThickness, 0, true);
						PDI->DrawLine(MidPoint + Offset, MidPoint + Segment.Binormal * LineDirectionLength + Offset, LineColor, SDPG_World, FTraversalNavDataRenderingHelpers::DefaultLineThickness, 0, true);
						PDI->DrawLine(MidPoint + Offset, MidPoint + Normal * LineDirectionLength + Offset, LineColor, SDPG_World, FTraversalNavDataRenderingHelpers::DefaultLineThickness, 0, true);
					}
				}
			}
		}
	}
}
  
FPrimitiveViewRelevance FTraversalNavSegmentDataRenderSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bDynamicRelevance = true;
	return Result;
}

bool FTraversalNavSegmentDataRenderSceneProxy::CanBeOccluded() const
{
	return false;
}


// UTraversalNavDataRenderingComponent -------------------------------------------------------------------------------------------------------------

UTraversalNavDataRenderingComponent::UTraversalNavDataRenderingComponent()
{

}

FPrimitiveSceneProxy* UTraversalNavDataRenderingComponent::CreateSceneProxy()
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	FTraversalNavSegmentDataRenderSceneProxy* RenderSceneProxy = nullptr;
	if (bEnableDrawing)
	{
		RenderSceneProxy = new FTraversalNavSegmentDataRenderSceneProxy(*this);
	}

	return RenderSceneProxy;
#else
	return nullptr;
#endif //!UE_BUILD_SHIPPING && !UE_BUILD_TEST
}

FBoxSphereBounds UTraversalNavDataRenderingComponent::CalcBounds(const FTransform& LocalToWorld) const
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	return FBox(FVector(-HALF_WORLD_MAX), FVector(HALF_WORLD_MAX));
#else
	return FBox(ForceInitToZero);
#endif //!UE_BUILD_SHIPPING && !UE_BUILD_TEST
}
