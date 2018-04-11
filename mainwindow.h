#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <OGRE/Ogre.h>
#include <QWidget>
#include <QKeyEvent>
#include <QMouseEvent>

class OgreView : public QWidget
{
    Q_OBJECT

public:
    OgreView(QWidget* parent );
    ~OgreView();
    //QSize minimumSizeHint() const;
    //QSize sizeHint() const;
protected:
    void setupView();//创建Ogre使用的内容，场景内容;
    void update();//通过它，手动更新场景内容
    void setupResources();//
    void createScene();
    void createLight();
    void resizeEvent(QResizeEvent* evt);//窗口可能缩放
    void timerEvent(QTimerEvent* evt);//可以设一时钟来更新窗口
    void paintEvent(QPaintEvent* evt);//呵，当然得处理了
            //消息响应

    void keyPressEvent(QKeyEvent* evt);
    void mousePressEvent(QMouseEvent* evt);
    void mouseReleaseEvent(QMouseEvent* evt);
    void wheelEvent(QWheelEvent* evt);

    //有关Ogre
    Ogre::RenderWindow* mRenderWindow;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mVp;
    Ogre::Root* mRoot;
    Ogre::Light* mainLight;
    Ogre::Entity* mainEnt;
    Ogre::SceneNode* mainNode;
    Ogre::Real angleX;
    Ogre::Real angleY;
    Ogre::Real rotX;
    Ogre::Real rotY;
    Ogre::Real mZoom;
    Ogre::Vector3 mDirection;
    Ogre::Real mRotate;
    Ogre::Vector2 mousePos;
    Ogre::RenderSystem *renderSys;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    bool mouseMiddleBtn;

};
#endif // MAINWINDOW_H
