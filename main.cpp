#include <QApplication>
#include <QWidget>
#include "mainwindow.h"
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;

    window.resize(800, 600);
    window.setWindowTitle("Simple example");

    OgreView* ogreWidget = new OgreView(&window);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ogreWidget);
    window.setLayout(layout);
    window.show();

    return app.exec();
}
