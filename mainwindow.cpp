#include "mainwindow.h"
#include <QX11Info>
#include <QDebug>

OgreView::OgreView(QWidget* parent) : QWidget(parent,Qt::WindowFlags(Qt::MSWindowsOwnDC))
{
    mRenderWindow = NULL;
    mSceneMgr = NULL;
    mVp = NULL;
    mainEnt = NULL;
    mainNode = NULL;
    angleX = 1;
    angleY = 1;
    rotX = 1;
    rotY = 1;
    mZoom = 1;
    mRotate = 1;
    mouseLeftPressed = false;
    mouseRightPressed = false;
    mouseMiddleBtn = false;
    setupResources();
}

OgreView::~OgreView()
{
    if( mVp )
    {
        mRenderWindow->removeViewport(mVp->getZOrder());
        mVp = 0;
    }
    Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);
    mRenderWindow = 0;

    if(mRoot != NULL)
    {
        delete mRoot;
        mRoot = 0;
    }
}


void OgreView::setupResources()
{
    mRoot = new Ogre::Root("/home/tony/work/ogre/OrgeProject/cfg/plugins.cfg");

    Ogre::ConfigFile cf;
    cf.load("/home/tony/work/ogre/OrgeProject/cfg/resources.cfg");
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while(seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i=settings->begin(); i!=settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    const Ogre::RenderSystemList *rsList;
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR == 6
    rsList = mRoot->getAvailableRenderers();
#else
    rsList = &(mRoot->getAvailableRenderers());
#endif
    renderSys = NULL;
    for(unsigned int i=0;i<rsList->size();i++ )
    {
        renderSys = rsList->at( i );
        if(renderSys->getName().compare("OpenGL Rendering Subsystem")== 0 )
        {
            break;
        }
    }

    if( renderSys == NULL )
    {
        throw std::runtime_error( "Could not find the opengl rendering subsystem!\n" );
    }
    renderSys->setConfigOption("Full Screen","No");
    mRoot->setRenderSystem(renderSys);
    mRoot->initialise(false);
}

void OgreView::setupView()
{
    if(mRenderWindow)
        return;

    Ogre::NameValuePairList params;
    QWidget *q_parent = dynamic_cast <QWidget *> (parent());
    params["parentWindowHandle"] = Ogre::StringConverter::toString ((unsigned long)QX11Info::display()) +
            ":" + Ogre::StringConverter::toString ((unsigned int)QX11Info::appScreen()) +
            ":" + Ogre::StringConverter::toString ((unsigned long)q_parent->winId());

    mRenderWindow = mRoot->createRenderWindow("View", width(), height(), false, &params);
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);

    mVp = mRenderWindow->addViewport(mCamera);
    mVp->setBackgroundColour(Ogre::ColourValue(0, 0.0, 0.0, 1));

    mCamera->setAspectRatio(Ogre::Real(mVp->getActualWidth()) / Ogre::Real(mVp->getActualHeight()));
    //resource
    //ERROR
    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    createScene();
    createLight();
}

void OgreView::createScene()
{
    mainEnt = mSceneMgr->createEntity("Head", "Sinbad.mesh");
    mainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mainNode->attachObject(mainEnt);
}

void OgreView::createLight()
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
    mainLight = mSceneMgr->createLight("MainLight");
    mainLight->setPosition(20,80,50);
}

void OgreView::paintEvent(QPaintEvent *evt)
{
    Q_UNUSED(evt);
    if(mRenderWindow == NULL)
        setupView();
    update();
}

void OgreView::timerEvent(QTimerEvent* evt)
{
    Q_UNUSED(evt);
    update();
}

void OgreView::update()
{
    if(mRenderWindow != NULL)
    {
        mRoot->renderOneFrame();
    }
}


void OgreView::resizeEvent(QResizeEvent *evt)
{
    Q_UNUSED(evt);
    if (mRenderWindow != NULL)
    {
        mRenderWindow->windowMovedOrResized();
        mCamera->setAspectRatio(Ogre::Real(mVp->getActualWidth()) / Ogre::Real(mVp->getActualHeight()));
    }
}

void OgreView::keyPressEvent(QKeyEvent* evt)
{
    qDebug("keypressevent");
    if(mainEnt != NULL && mainNode != NULL)
    {
        switch(evt->key())
        {
        case Qt::Key_W:
        case Qt::Key_Up:
        {
            qDebug("W/UP");
            rotX = -0.1;
            mainNode->pitch(Ogre::Radian(rotX));

            break;
        }
        case Qt::Key_S:
        case Qt::Key_Down:
        {
            qDebug("S/Down");
            rotX = 0.1;
            mainNode->pitch(Ogre::Radian(rotX));
            break;
        }
        case Qt::Key_A:
        case Qt::Key_Left:
        {
            qDebug("A/Left");
            rotY = -0.1;
            mainNode->yaw(Ogre::Radian(rotY));
            break;
        }
        case Qt::Key_D:
        case Qt::Key_Right:
        {
            qDebug("D/Right");
            rotY = 0.1;
            mainNode->yaw(Ogre::Radian(rotY));
            break;
        }
        }
    }
}


void OgreView::mousePressEvent(QMouseEvent* evt)
{
    qDebug("mousePressEvent");
    if(evt->button() == Qt::LeftButton)
    {
        mouseLeftPressed = true;
    }
    if(evt->button() == Qt::RightButton)
    {
        mouseRightPressed = true;
        mousePos = Ogre::Vector2(evt->x(), evt->y());
    }
    if(evt->button() == Qt::MidButton)
    {
        mouseMiddleBtn = true;
    }
}

void OgreView::mouseReleaseEvent(QMouseEvent *evt)
{
    qDebug("mouseReleaseEvent");
    if(evt->button() == Qt::LeftButton)
    {
        mouseLeftPressed = false;
    }
    if(evt->button() == Qt::RightButton)
    {
        mouseRightPressed = false;
        mousePos = Ogre::Vector2(evt->x(), evt->y());
    }
    if(evt->button() == Qt::MidButton)
    {
        mouseMiddleBtn = false;
    }
}

void OgreView::wheelEvent(QWheelEvent* evt)
{
    if(evt->delta()>0){
        mainNode->setPosition(0,0,mZoom++);
    }
    else{
        mainNode->setPosition(0,0,mZoom--);
    }
    update();
}
