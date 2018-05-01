#pragma once

#include "engine/graphics/Resources.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/CommonNoDep.h"
#include "shaders/core/culling/PrefixSumBuckets.h"
#include "shaders/core/culling/PrefixSumBucketResults.h"
#include "shaders/core/culling/PrefixSumAddBuckets.h"
#include <vector>
#include <string>

namespace engine
{
    class Device;
    class ShaderStorage;
    class PrefixSumClusters
    {
    public:
        PrefixSumClusters(Device& device, ShaderStorage& shaderStorage);
        void prefixSum(
            Device& device,
            CommandList& cmd,
            BufferSRV& clusters,
            BufferSRV& clusterCounts,
            Buffer& args,
            BufferUAV& results);
    private:
        engine::Pipeline<shaders::PrefixSumBuckets> m_buckets;
        engine::Pipeline<shaders::PrefixSumBucketResults> m_bucketResults;
        engine::Pipeline<shaders::PrefixSumAddBuckets> m_addbuckets;

        BufferUAV m_bucketBufferUAV;
        BufferSRV m_bucketBufferSRV;

        BufferUAV m_bucketSumBufferUAV;
        BufferSRV m_bucketSumBufferSRV;
    };
}
