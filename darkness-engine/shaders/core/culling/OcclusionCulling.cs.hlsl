
#include "../Common.hlsli"
#include "../shared_types/ClusterInstanceData.hlsli"
#include "../shared_types/BoundingBox.hlsli"
#include "../shared_types/TransformHistory.hlsli"

StructuredBuffer<ClusterInstanceData> frustumCullingOutput;
StructuredBuffer<BoundingBox> boundingBoxes;
StructuredBuffer<TransformHistory> transformHistory;
RWStructuredBuffer<ClusterInstanceData> cullingOutput;
Buffer<uint> clusterCount;
RWBuffer<uint> occlusionDrawArgs;
RWBuffer<uint> distributor;

Texture2D<float> depthPyramid;
sampler depthSampler;

cbuffer CullingConstants
{
    // 16
    float4x4 viewMatrix;

    // 16
    float4x4 projectionMatrix;

    // 16
    float4 cameraPosition;

    float2 size;
    float2 inverseSize;

    float2 pow2size;
    float2 nearFar;

    float4 mipLevels;

    // 16
    float4 plane0;
    float4 plane1;
    float4 plane2;
    float4 plane3;

    float4 plane4;
    float4 plane5;
};

float4 transformBoundingSphere(float4x4 mat, float4 boundingSphere)
{
    float3 pos = mul(mat, float4(boundingSphere.xyz, 1.0f)).xyz - cameraPosition.xyz;
    float3 radiusLong = mul(mat, float4(boundingSphere.w + boundingSphere.w, 0.0f, 0.0f, 1.0f)).xyz;
    float3 radiusShort = mul(mat, float4(boundingSphere.w, 0.0f, 0.0f, 1.0f)).xyz;
    float radius = length(radiusLong - radiusShort);
    return float4(pos, radius);
};

bool frustumCull(float3 bbmin, float3 bbmax, float4x4 transform, float3 cameraPos)
{
    float3 corner[8] =
    {
        mul(transform, float4(bbmin.xyz, 1.0f)).xyz,
        mul(transform, float4(bbmin.x, bbmax.y, bbmax.z, 1.0f)).xyz,
        mul(transform, float4(bbmin.x, bbmin.y, bbmax.z, 1.0f)).xyz,
        mul(transform, float4(bbmin.x, bbmax.y, bbmin.z, 1.0f)).xyz,
        mul(transform, float4(bbmax, 1.0f)).xyz,
        mul(transform, float4(bbmax.x, bbmin.y, bbmin.z, 1.0f)).xyz,
        mul(transform, float4(bbmax.x, bbmax.y, bbmin.z, 1.0f)).xyz,
        mul(transform, float4(bbmax.x, bbmin.y, bbmax.z, 1.0f)).xyz
    };

    float3 frustumPlanes[6] =
    {
        plane0.xyz,
        plane1.xyz,
        plane2.xyz,
        plane3.xyz,
        plane4.xyz,
        plane5.xyz
    };

    for (int i = 0; i < 6; ++i)
    {
        bool hit = false;
        for (int c = 0; c < 8; ++c)
        {
            if (dot(normalize(frustumPlanes[i]), (corner[c] - cameraPos)) >= 0)
            {
                hit = true;
                break;
            }
        }
        if (!hit)
            return false;
    }
    return true;
};

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    if (dispatchThreadID.x < clusterCount[0])
    {
        ClusterInstanceData cInstanceData = frustumCullingOutput[dispatchThreadID.x];
        TransformHistory transforms = transformHistory[cInstanceData.instancePointer];

        float4x4 toCamera = mul(viewMatrix, transforms.transform);
        
        BoundingBox bb = boundingBoxes[cInstanceData.clusterPointer];
        float3 bbmin = bb.min;
        float3 bbmax = bb.max;

        if (frustumCull(bbmin, bbmax, transforms.transform, cameraPosition.xyz))
        {

            float3 corner[8] =
            {
                mul(toCamera, float4(bbmin.xyz, 1.0f)).xyz,
                mul(toCamera, float4(bbmin.x, bbmax.y, bbmax.z, 1.0f)).xyz,
                mul(toCamera, float4(bbmin.x, bbmin.y, bbmax.z, 1.0f)).xyz,
                mul(toCamera, float4(bbmin.x, bbmax.y, bbmin.z, 1.0f)).xyz,
                mul(toCamera, float4(bbmax, 1.0f)).xyz,
                mul(toCamera, float4(bbmax.x, bbmin.y, bbmin.z, 1.0f)).xyz,
                mul(toCamera, float4(bbmax.x, bbmax.y, bbmin.z, 1.0f)).xyz,
                mul(toCamera, float4(bbmax.x, bbmin.y, bbmax.z, 1.0f)).xyz
            };

            float4 cornerScreen[8] =
            {
                mul(projectionMatrix, float4(corner[0], 1.0f)),
                mul(projectionMatrix, float4(corner[1], 1.0f)),
                mul(projectionMatrix, float4(corner[2], 1.0f)),
                mul(projectionMatrix, float4(corner[3], 1.0f)),

                mul(projectionMatrix, float4(corner[4], 1.0f)),
                mul(projectionMatrix, float4(corner[5], 1.0f)),
                mul(projectionMatrix, float4(corner[6], 1.0f)),
                mul(projectionMatrix, float4(corner[7], 1.0f))
            };

            cornerScreen[0].xyz = cornerScreen[0].xyz / cornerScreen[0].w;
            cornerScreen[1].xyz = cornerScreen[1].xyz / cornerScreen[1].w;
            cornerScreen[2].xyz = cornerScreen[2].xyz / cornerScreen[2].w;
            cornerScreen[3].xyz = cornerScreen[3].xyz / cornerScreen[3].w;
            cornerScreen[4].xyz = cornerScreen[4].xyz / cornerScreen[4].w;
            cornerScreen[5].xyz = cornerScreen[5].xyz / cornerScreen[5].w;
            cornerScreen[6].xyz = cornerScreen[6].xyz / cornerScreen[6].w;
            cornerScreen[7].xyz = cornerScreen[7].xyz / cornerScreen[7].w;

            cornerScreen[0].xy = ((cornerScreen[0].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[1].xy = ((cornerScreen[1].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[2].xy = ((cornerScreen[2].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[3].xy = ((cornerScreen[3].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[4].xy = ((cornerScreen[4].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[5].xy = ((cornerScreen[5].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[6].xy = ((cornerScreen[6].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));
            cornerScreen[7].xy = ((cornerScreen[7].xy * float2(0.5, 0.5)) + float2(0.5, 0.5));

            // flip y
            cornerScreen[0].y = 1.0f - cornerScreen[0].y;
            cornerScreen[1].y = 1.0f - cornerScreen[1].y;
            cornerScreen[2].y = 1.0f - cornerScreen[2].y;
            cornerScreen[3].y = 1.0f - cornerScreen[3].y;
            cornerScreen[4].y = 1.0f - cornerScreen[4].y;
            cornerScreen[5].y = 1.0f - cornerScreen[5].y;
            cornerScreen[6].y = 1.0f - cornerScreen[6].y;
            cornerScreen[7].y = 1.0f - cornerScreen[7].y;

            cornerScreen[0].xy *= size;
            cornerScreen[1].xy *= size;
            cornerScreen[2].xy *= size;
            cornerScreen[3].xy *= size;
            cornerScreen[4].xy *= size;
            cornerScreen[5].xy *= size;
            cornerScreen[6].xy *= size;
            cornerScreen[7].xy *= size;

            // these are actual screen coordinates
            float2 minXY = min(
                min(min(cornerScreen[0].xy, cornerScreen[1].xy), min(cornerScreen[2].xy, cornerScreen[3].xy)),
                min(min(cornerScreen[4].xy, cornerScreen[5].xy), min(cornerScreen[6].xy, cornerScreen[7].xy)));
            float2 maxXY = max(
                max(max(cornerScreen[0].xy, cornerScreen[1].xy), max(cornerScreen[2].xy, cornerScreen[3].xy)),
                max(max(cornerScreen[4].xy, cornerScreen[5].xy), max(cornerScreen[6].xy, cornerScreen[7].xy)));
            float clusterMaxDepth = max(
                max(max(cornerScreen[0].z, cornerScreen[1].z), max(cornerScreen[2].z, cornerScreen[3].z)),
                max(max(cornerScreen[4].z, cornerScreen[5].z), max(cornerScreen[6].z, cornerScreen[7].z)));

            bool somethingVisible = false;
            float2 qSize = (maxXY - minXY) / 4.0f;
            for (int x = 0; x < 4; ++x)
            {
                for (int y = 0; y < 4; ++y)
                {
                    float2 localMinXY = minXY + float2(qSize.x * x, qSize.y * y);
                    float2 localMaxXY = localMinXY + qSize;

                    float2 UVminXY = saturate(localMinXY / pow2size);
                    float2 UVmaxXY = saturate(localMaxXY / pow2size);

                    float biggerDimension = max(qSize.x, qSize.y);
                    uint mip = log2(biggerDimension) + 1;

                    float4 blockDepths = float4(
                        depthPyramid.SampleLevel(depthSampler, float2(UVminXY.x, UVminXY.y), mip),
                        depthPyramid.SampleLevel(depthSampler, float2(UVmaxXY.x, UVminXY.y), mip),
                        depthPyramid.SampleLevel(depthSampler, float2(UVminXY.x, UVmaxXY.y), mip),
                        depthPyramid.SampleLevel(depthSampler, float2(UVmaxXY.x, UVmaxXY.y), mip)
                        );

                    float minBlockDepth = min(min(blockDepths.x, blockDepths.y), min(blockDepths.z, blockDepths.w));

                    if (linearDepth(clusterMaxDepth, float2(0.1, 1000.0)) >
						linearDepth(minBlockDepth, float2(0.1, 1000.0)) - 1.0)
                    {
                        somethingVisible = true;
                        break;
                    }
                }
            }

			if (minXY.x < 0 ||
				minXY.y < 0 || 
				maxXY.x > size.x ||
				maxXY.y > size.y)
				somethingVisible = true;

#ifdef OPTION_EMIT_ALL
#else
            if (somethingVisible)
#endif
            {
                uint newIndex = 0;
                InterlockedAdd(distributor[0], 1, newIndex);
                cullingOutput[newIndex] = cInstanceData;

                InterlockedAdd(occlusionDrawArgs[0], 192);
            }
        }

        if (dispatchThreadID.x == 0)
        {
            occlusionDrawArgs[1] = 1;
            occlusionDrawArgs[2] = 1;
            occlusionDrawArgs[3] = 0;
        }
    }
}
