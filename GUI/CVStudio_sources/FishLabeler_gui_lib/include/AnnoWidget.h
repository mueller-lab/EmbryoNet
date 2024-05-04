//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================


#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QWheelEvent>


class AnnoWidget: public QOpenGLWidget, protected QOpenGLFunctions
{

    Q_OBJECT

public:

    AnnoWidget(QWidget* parent = nullptr);
    ~AnnoWidget();

    void setImage(const QImage& img);
    void setArrows(const QList<QVector2D>& positions, const QList<QVector3D>& rotations);

    void resizeEvent(QResizeEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
   
signals:

    void sendLeftClick(const QPoint& p);
    void sendWheelRotation(const QPoint& pos, const QPoint angleDelta);
    void wheelPress(const QPoint& p);
    void sendRightClick(const QPoint& p);
   //void sendKey( const QKeyEvent *p);



private:

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void updateFrame(const QImage& frame);

    bool loadShaderFromFile(QOpenGLShaderProgram* program, 
                            QOpenGLShader::ShaderType type,
                            const QString& filePath);
    
    QString readFile(const QString& filePath);
    QOpenGLTexture* createTexture(const QImage& image);

private:

    QImage m_img;

    QOpenGLTexture* m_frame;
    QOpenGLShaderProgram* m_program_arrows;
    QOpenGLShaderProgram* m_program_boxes;
    QOpenGLShaderProgram* m_program_frame;

    QOpenGLVertexArrayObject m_VAO_quad, m_VAO_1;
    QOpenGLBuffer m_VBO_quad, m_VBO_1;

    QList<QVector3D> m_rotations;
    QList<QVector2D> m_positions;

    int m_width = 0, m_height = 0;
};
