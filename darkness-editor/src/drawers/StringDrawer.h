#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "tools/Property.h"
#include "engine/Drawer.h"
#include "DragableLabel.h"
#include "engine/primitives/Vector3.h"

namespace engine
{
	class DroppableLabel : public QLineEdit
	{
		Q_OBJECT
	public:
		DroppableLabel(QWidget* parent = Q_NULLPTR);
		void setPaths(const std::string& contentPath, const std::string& processedContentPath);
	signals:
		void onChanged();
	protected:
		void dropEvent(QDropEvent *dropEvent) override;
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dragLeaveEvent(QDragLeaveEvent *event) override;
		void dragMoveEvent(QDragMoveEvent *event) override;
	private:
		std::string m_contentPath;
		std::string m_processedContentPath;
	};

    class StringWidget : public QWidget
    {
        Q_OBJECT
    public:
        StringWidget(Property& value, QWidget *parent = Q_NULLPTR,
            Qt::WindowFlags f = Qt::WindowFlags());

		void setPaths(const std::string& contentPath, const std::string& processedContentPath);

        virtual ~StringWidget();
    private:
        Property& m_value;
        bool m_valueValid;
        bool m_canReceiveUpdate;
		std::string m_contentPath;
		std::string m_processedContentPath;
        std::unique_ptr<QHBoxLayout> m_layout;

        std::unique_ptr<QLabel> m_nameLabel;
        std::unique_ptr<DroppableLabel> m_xLine;
        std::unique_ptr<DragableLabel> m_xLabel;

        void updateProperty();

    private slots:
        void onEditFinished();
    };

    class StringDrawer : public Drawer
    {
    public:
        StringDrawer(Property& value);
        void setParent(void* parent) override;
        void* native() override;
		void setPaths(const std::string& contentPath, const std::string& processedContentPath) override;

    private:
        Property& m_value;
		std::string m_contentPath;
		std::string m_processedContentPath;
        std::shared_ptr<QWidget> m_widget;
        QWidget* m_parent;
    };

    template <>
    std::shared_ptr<Drawer> createDrawer<std::string>(Property& value);

}
