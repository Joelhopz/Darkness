#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "DrawerCallbacks.h"
#include "Property.h"
#include "engine/primitives/Rect.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Matrix4.h"

class TextBox : public Visual
{
    const Property& m_property;
    DrawerCallbacks& m_call;
public:
    TextBox(DrawerCallbacks& call, const Property& prop)
        : m_property{ prop }
        , m_call{ call }
    {}
    void draw(engine::Rect rect) override
    {
        m_call.drawString(m_call.propertyWidget, rect, m_property.value<std::string>());
    }
};

enum class LayoutType
{
    Horizontal,
    Vertical
};

class VisualContainer
{
public:
    void addItem(std::shared_ptr<Visual> visual)
    {
        m_childs.emplace_back(visual);
    }
    std::vector<std::shared_ptr<Visual>>& items()
    {
        return m_childs;
    }
protected:
    std::vector<std::shared_ptr<Visual>> m_childs;
};

class Layout : public Visual,
               public VisualContainer
{
    LayoutType m_type;
public:
    Layout(LayoutType type)
        : m_type{ type }
    {}

    void setLayoutType(LayoutType type)
    {
        m_type = type;
    }

    void draw(engine::Rect rect) override
    {
        int childCount = static_cast<int>(m_childs.size());
        int width = m_type == LayoutType::Horizontal ? rect.width / childCount : rect.width;
        int height = m_type == LayoutType::Vertical ? rect.height / childCount : rect.height;

        int correctLastWidth = 0;
        int correctLastHeight = 0;
        if (m_type == LayoutType::Horizontal && width * childCount != rect.width)
            correctLastWidth = rect.width - (width * childCount);
        if (m_type == LayoutType::Vertical && height * childCount != rect.height)
            correctLastHeight = rect.height - (height * childCount);

        int currentX = rect.x;
        int currentY = rect.y;
        for (int i = 0; i < childCount; ++i)
        {
            engine::Rect childRect{ currentX, currentY, width, height };
            if (i == childCount - 1)
            {
                if (m_type == LayoutType::Horizontal)
                    childRect.width += correctLastWidth;
                if (m_type == LayoutType::Vertical)
                    childRect.height += correctLastHeight;
            }
            m_childs[i]->draw(childRect);

            if(m_type == LayoutType::Horizontal)
                currentX += width;
            if(m_type == LayoutType::Vertical)
                currentY += height;
        }
    }
};

class IntDrawer : public Visual
{
    DrawerCallbacks m_callbacks;
    const int& m_value;
public:
    IntDrawer(const int& value, DrawerCallbacks callbacks)
        : m_callbacks{ callbacks }
        , m_value{ value }
    {}
    void draw(engine::Rect rect) override
    {
        m_callbacks.drawString(m_callbacks.propertyWidget, rect, std::to_string(m_value));
    }
};

class FloatDrawer : public Visual
{
    DrawerCallbacks m_callbacks;
    const float& m_value;
public:
    FloatDrawer(const float& value, DrawerCallbacks callbacks)
        : m_callbacks{ callbacks }
        , m_value{ value }
    {}
    void draw(engine::Rect rect) override
    {
        m_callbacks.drawString(m_callbacks.propertyWidget, rect, std::to_string(m_value));
    }
};

class StdStringDrawer : public Visual
{
    DrawerCallbacks m_callbacks;
    const std::string& m_value;
public:
    StdStringDrawer(const std::string& value, DrawerCallbacks callbacks)
        : m_callbacks{ callbacks }
        , m_value{ value }
    {}
    void draw(engine::Rect rect) override
    {
        m_callbacks.drawString(m_callbacks.propertyWidget, rect, m_value);
    }
};


class DragableValueDrawer : public Visual
{
    DrawerCallbacks m_callbacks;
    const std::string& m_value;
public:
    DragableValueDrawer(const std::string& value, DrawerCallbacks callbacks)
        : m_callbacks{ callbacks }
        , m_value{ value }
    {}
    void draw(engine::Rect rect) override
    {
        m_callbacks.drawString(m_callbacks.propertyWidget, rect, m_value);
    }
};

class TextEditDrawer : public Visual
{
    DrawerCallbacks m_callbacks;
    const std::string& m_value;
public:
    TextEditDrawer(const std::string& value, DrawerCallbacks callbacks)
        : m_callbacks{ callbacks }
        , m_value{ value }
    {}
    void draw(engine::Rect rect) override
    {
        m_callbacks.drawString(m_callbacks.propertyWidget, rect, m_value);
    }
};

class Vector3fDrawer : public Layout
{
    DrawerCallbacks m_callbacks;
    const engine::Vector3f& m_value;

public:
    Vector3fDrawer(const engine::Vector3f& value, DrawerCallbacks callbacks)
        : Layout{ LayoutType::Horizontal }
        , m_callbacks{ callbacks }
        , m_value{ value }
    {
        auto labelLayout = std::make_shared<Layout>(LayoutType::Horizontal);
        auto valueLayout = std::make_shared<Layout>(LayoutType::Horizontal);
        addItem(labelLayout);
        addItem(valueLayout);

        labelLayout->addItem(std::make_shared<StdStringDrawer>("vector name", callbacks));
        
        valueLayout->addItem(std::make_shared<DragableValueDrawer>(" ", callbacks));
        valueLayout->addItem(std::make_shared<TextEditDrawer>("", callbacks));

        valueLayout->addItem(std::make_shared<DragableValueDrawer>(" ", callbacks));
        valueLayout->addItem(std::make_shared<TextEditDrawer>("", callbacks));

        valueLayout->addItem(std::make_shared<DragableValueDrawer>(" ", callbacks));
        valueLayout->addItem(std::make_shared<TextEditDrawer>("", callbacks));
    }
};

class Matrix4fDrawer : public Visual
{
    DrawerCallbacks m_callbacks;
    const engine::Matrix4f& m_value;
public:
    Matrix4fDrawer(const engine::Matrix4f& value, DrawerCallbacks callbacks)
        : m_callbacks{ callbacks }
        , m_value{ value }
    {}
    void draw(engine::Rect /*rect*/) override
    {
        //m_callbacks.drawString(m_callbacks.propertyWidget, rect, std::to_string(m_value));
    }
};


