#pragma once

#include "engine/EngineComponent.h"
#include "engine/rendering/Material.h"
#include "tools/Property.h"
#include "tools/PathTools.h"
#include "tools/Settings.h"
#include "tools/AssetTools.h"
#include "tools/image/Image.h"
#include <memory>

namespace engine
{
    class ImagePropertiesComponent : public EngineComponent
    {
    public:
        ImagePropertiesComponent() = delete;

        ImagePropertiesComponent(const std::string& path)
            : m_path{ path }
            , m_settings{ pathReplaceExtension(path, "json") }
            , m_image{ engine::image::Image::createImage(path, image::ImageType::DDS) }
            , m_size{ this, "size", 
                m_image ? m_settings.get<int>("size", static_cast<int>(m_image->width())) : 0, [this]() { m_settings.set<int>("size", m_size.value<int>()); } }
            , m_textureType{ this, "type", textureTypeFromString(m_settings.get<std::string>("textureType")), [this]() 
            {
                m_settings.set<std::string>("textureType", textureTypeToString(m_textureType.value<TextureType>()));
                // reset user defined texture encoding with a recommended format
                this->m_textureEncoding.value<std::string>(formatToString(possibleEncodingFormatRGB(m_textureType.value<TextureType>())[0]));
            }}
            , m_textureEncoding{ this, "encoding", m_settings.get<std::string>("textureEncoding"), [this]() { m_settings.set<std::string>("textureEncoding", m_textureEncoding.value<std::string>()); } }
            , m_generateMips{ this, "generateMips", static_cast<ButtonToggle>(m_settings.get<bool>("generateMips", static_cast<bool>(ButtonToggle::NotPressed))),
                [this]() 
                { 
                    bool generate = static_cast<bool>(this->m_generateMips.value<ButtonToggle>());
                    m_settings.set<bool>("generateMips", generate);
                    m_settings.save();
                } }
            , m_apply{ this, "apply", ButtonPush{ButtonPush::NotPressed}, [this]()
                {
                    if (this->m_apply.value<ButtonPush>() == ButtonPush::Pressed)
                        this->save();
                }
            }
        {
            //ASSERT(m_settings.get<std::string>("sourcePath") != "", "There should be no way to get ImageProperties open without settings file");
            //ASSERT(m_settings.get<std::string>("destinationPath") != "", "There should be no way to get ImageProperties open without settings file");
            m_name = "ImagePropertiesComponent";
        }

        std::shared_ptr<engine::EngineComponent> clone() const override
        {
            return std::make_shared<ImagePropertiesComponent>(m_path);
        }

        void start() override
        {
        }

        int size() const;
        void size(int _size);

        TextureType textureType() const;
        void textureType(TextureType _type);

        ButtonToggle generateMips() const;
        void generateMips(ButtonToggle val);

    private:
        tools::Settings m_settings;
        std::shared_ptr<engine::image::ImageIf> m_image;
        std::string m_path;

        void save()
        {
            m_settings.save();

            AssetNotification notification;
            notification.task = AssetTask::ImageRecompress;
            notification.sourcePath = m_settings.get<std::string>("sourcePath");
            notification.destinationPath = m_settings.get<std::string>("destinationPath");
            notification.encodeType = formatFromString(m_settings.get<std::string>("textureEncoding"));
            GlobalAssetNotificationService.sendNotification(notification);
        };


        Property m_size;
        Property m_textureType;
        Property m_textureEncoding;
        Property m_generateMips;
        Property m_apply;
    };
}
