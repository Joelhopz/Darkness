#include "engine/graphics/ShaderSupport.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#undef max
#undef min
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

namespace engine
{
    namespace implementation
    {
        ShaderSupport::ShaderSupport(const std::string& supportFilePath)
        {
            // read support
            ifstream supportFile;
            supportFile.open(supportFilePath, ios::in);
            if (supportFile.is_open())
            {
                auto begin = supportFile.tellg();
                supportFile.seekg(0, ios::end);
                auto end = supportFile.tellg();
                size_t size = static_cast<size_t>(end - begin);

                std::vector<char> supportData(size);

                supportFile.seekg(0, ios::beg);
                supportFile.read(supportData.data(), end - begin);
                supportFile.close();

                StringStream ss(supportData.data());
                Document document;
                document.ParseStream(ss);

                auto propertyString = [&document](const char* option, std::string& target)
                {
                    if (document.HasMember(option) && document[option].IsString())
                    {
                        target = document[option].GetString();
                    }
                    return "";
                };
                propertyString("binary_file", binaryFile);
                propertyString("executable", executable);
                propertyString("file", file);
                propertyString("graphics_api", graphicsApi);
                propertyString("shader_compiler_path", shaderCompilerPath);
                propertyString("root_path", rootPath);
            }
        }
    }
}
