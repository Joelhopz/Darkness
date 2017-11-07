#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "tools/Property.h"
#include "engine/Drawer.h"
#include "DragableLabel.h"
#include "engine/primitives/Matrix4.h"
#include "engine/primitives/Quaternion.h"

namespace engine
{
    class Matrix4fWidget : public QWidget
    {
        Q_OBJECT
    public:
        Matrix4fWidget(Property& value, bool matrix, QWidget *parent = Q_NULLPTR,
            Qt::WindowFlags f = Qt::WindowFlags());

        virtual ~Matrix4fWidget();
    private:
        Property& m_value;
        bool m_valueValid;
        bool m_canReceiveUpdate;
        bool m_matrix;
        std::unique_ptr<QHBoxLayout> m_layout;

        std::unique_ptr<QLabel> m_nameLabel;
        std::unique_ptr<QLineEdit> m_xLine;
        std::unique_ptr<DragableLabel> m_xLabel;
        std::unique_ptr<QLineEdit> m_yLine;
        std::unique_ptr<DragableLabel> m_yLabel;
        std::unique_ptr<QLineEdit> m_zLine;
        std::unique_ptr<DragableLabel> m_zLabel;

        void updateProperty();

    private slots:
        void onXEditFinished();
        void onYEditFinished();
        void onZEditFinished();
    };

    class Matrix4fDrawer : public Drawer
    {
    public:
        Matrix4fDrawer(Property& value, bool matrix);
        void setParent(void* parent) override;
        void* native() override;

    private:
        Property& m_value;
        bool m_matrix;
        std::shared_ptr<QWidget> m_widget;
        QWidget* m_parent;
    };

    template <>
    std::shared_ptr<Drawer> createDrawer<Matrix4f>(Property& value);

    template <>
    std::shared_ptr<Drawer> createDrawer<Quaternionf>(Property& value);
}
