#include <QWidget>

#include <RotationWidget.h>
#include <EmbryoCounterWidget.h>

#include <QSpacerItem>
#include <QSplitter>
#include <QVBoxLayout>

class RotationClassWidget : public QWidget
{
	Q_OBJECT
public:
	
	RotationClassWidget(QWidget* parent = nullptr);
	~RotationClassWidget();

	EmbryoCounter* embryoCounter() const;
	RotationWidget* rotationWidget() const;

private:

	QVBoxLayout* m_layout = nullptr;

	EmbryoCounter* m_embryoCounterWidget = nullptr;
	RotationWidget* m_rotationWidget = nullptr;

	QSpacerItem* m_annotationRotationSpacerItem = nullptr;
	QSplitter* m_splitter = nullptr;

signals:

	void send_rot(const QVector3D& rotation);

};