#include <RotationWidget.h>
#include <QVBoxLayout>
#include <QHBoxLayout>




RotationWidget::RotationWidget()
{
    allocate();
    draw();
    connectInternal();
}


RotationWidget::~RotationWidget()
{
   
}

void RotationWidget::resetRotation()
{

}

void RotationWidget::connectInternal()
{
    QObject::connect
    (
        this->m_add_x_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_x(this->x + 1);        
    }
    );
    QObject::connect
    (
        this->m_add_5x_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_x(this->x + 5);
    }
    );

    QObject::connect
    (
        this->m_add_y_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_y(this->y + 1);
    }
    );

    QObject::connect
    (
        this->m_add_5y_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_y(this->y + 5);
    }
    );

    QObject::connect
    (
        this->m_add_z_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_z(this->z + 1);
    }
    );

    QObject::connect
    (
        this->m_add_5z_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_z(this->z + 5);
    }
    );

    QObject::connect
    (
        this->m_subtract_x_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_x(this->x - 1);
    }
    );
    QObject::connect
    (
        this->m_subtract_5x_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_x(this->x - 5);
    }
    );

    QObject::connect
    (
        this->m_subtract_y_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_y(this->y - 1);
    }
    );
    QObject::connect
    (
        this->m_subtract_5y_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_y(this->y - 5);
    }
    );

    QObject::connect
    (
        this->m_subtract_z_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_z(this->z - 1);
    }
    );
    QObject::connect
    (
        this->m_subtract_5z_button,
        &QPushButton::clicked,
        [this]()
    {
        this->set_z(this->z - 5);
    }
    );
   

}


void RotationWidget::set_x(const int& val)
{
    this->x = val;
    this->m_xLineEdit->setText(QString::number(this->x));
}

void RotationWidget::set_y(const int& val)
{
    this->y = val;
    this->m_yLineEdit->setText(QString::number(this->y));
}

void RotationWidget::set_z(const int& val)
{
    this->z = val;
    this->m_zLineEdit->setText(QString::number(this->z));
}


void RotationWidget::allocate()
{
    m_mainLayout = new QVBoxLayout();
    x_Layout = new QHBoxLayout();
    y_Layout = new QHBoxLayout();
    z_Layout = new QHBoxLayout();
    x_label = new QLabel(QString("x: "));
    y_label = new QLabel(QString("y: "));;
    z_label = new QLabel(QString("z: "));;

    m_xLineEdit = new QLineEdit("0");
    m_yLineEdit = new QLineEdit("0");
    m_zLineEdit = new QLineEdit("0");

    m_add_x_button = new QPushButton("+");
    m_add_y_button = new QPushButton("+");
    m_add_z_button = new QPushButton("+");
    m_add_5x_button = new QPushButton("+5");
    m_add_5y_button = new QPushButton("+5");
    m_add_5z_button = new QPushButton("+5");
    

    m_subtract_x_button = new QPushButton("-");
    m_subtract_y_button = new QPushButton("-");
    m_subtract_z_button = new QPushButton("-");
    m_subtract_5x_button = new QPushButton("-5");
    m_subtract_5y_button = new QPushButton("-5");
    m_subtract_5z_button = new QPushButton("-5");

}

void RotationWidget::draw()
{
    x_Layout->addWidget(x_label);
    x_Layout->addWidget(m_subtract_5x_button);
    x_Layout->addWidget(m_subtract_x_button);
    x_Layout->addWidget(m_xLineEdit);
    x_Layout->addWidget(m_add_x_button);
    x_Layout->addWidget(m_add_5x_button);

    y_Layout->addWidget(y_label);
    y_Layout->addWidget(m_subtract_5y_button);
    y_Layout->addWidget(m_subtract_y_button);
    y_Layout->addWidget(m_yLineEdit);
    y_Layout->addWidget(m_add_y_button);
    y_Layout->addWidget(m_add_5y_button);

    z_Layout->addWidget(z_label);
    z_Layout->addWidget(m_subtract_5z_button);
    z_Layout->addWidget(m_subtract_z_button);
    z_Layout->addWidget(m_zLineEdit);
    z_Layout->addWidget(m_add_z_button);
    z_Layout->addWidget(m_add_5z_button);
   
    m_mainLayout->addLayout(x_Layout);
    m_mainLayout->addLayout(y_Layout);
    m_mainLayout->addLayout(z_Layout);
    this->setLayout(m_mainLayout);
};
