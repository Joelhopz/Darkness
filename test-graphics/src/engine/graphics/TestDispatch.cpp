#include "GlobalTestFixture.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"

#include "shaders/core/tests/HLSLTest.h"
#include "shaders/core/tests/DrawWithoutBuffers.h"
#include "shaders/core/tests/DrawWithConstants.h"
#include "shaders/core/tests/DrawWithPixelShaderResources.h"
#include "shaders/core/tests/DrawWithTexture.h"
#include "shaders/core/tests/DispatchBasic.h"

#include <iostream>
#include <fstream>

TEST(TestDraw, DispatchBasic)
{
    GlobalEnvironment& env = *envPtr;

    auto inputElementCount = 150;
    auto outputElementCount = 250;

    auto basicDispatch = env.device().createPipeline<DispatchBasic>(env.shaderStorage());
    basicDispatch.cs.outputs = env.device().createBufferUAV(BufferDescription().elements(250).format(Format::R32_UINT).name("Dispatch output"));
    basicDispatch.cs.size.x = inputElementCount;

    std::vector<uint32_t> signal;
    for (int i = 0; i < inputElementCount; ++i) { signal.emplace_back(i); }

    basicDispatch.cs.inputs = env.device().createBufferSRV(BufferDescription()
        .format(Format::R32_UINT)
        .name("Dispatch input")
        .setInitialData(BufferDescription::InitialData(signal)));

    do
    {
        auto cmdBuffer = env.device().createCommandList();

        cmdBuffer.clearBuffer(basicDispatch.cs.outputs.buffer(), 0, 0, outputElementCount);

        cmdBuffer.bindPipe(basicDispatch);
        cmdBuffer.dispatch(roundUpToMultiple(static_cast<int>(inputElementCount), static_cast<size_t>(64)) / 64, 1, 1);

        env.submit(std::move(cmdBuffer));
        env.present();
    } while (env.canContinue(false));
    env.device().queue().waitForIdle();
}
