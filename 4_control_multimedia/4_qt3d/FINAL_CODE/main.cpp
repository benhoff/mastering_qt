#include <QApplication>
#include <QWidget>
#include <QScreen>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/QCamera>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/QSphereMesh>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor("white"));

    QWidget *container = QWidget::createWindowContainer(view);
    QSize screen_size = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screen_size);

    Qt3DCore::QEntity *root_entity = new Qt3DCore::QEntity();

   Qt3DRender::QCamera *camera_entity = view->camera();
   camera_entity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
   camera_entity->setPosition(QVector3D(0, 0, 20.0f));
   camera_entity->setUpVector(QVector3D(0, 1, 0));
   camera_entity->setViewCenter(QVector3D(0, 0, 0));

   Qt3DCore::QEntity *light_entity = new Qt3DCore::QEntity(root_entity);
   Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(light_entity);
   light->setColor("white");
   light->setIntensity(1);
   light_entity->addComponent(light);

   Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(light_entity);
   lightTransform->setTranslation(camera_entity->position());
   light_entity->addComponent(lightTransform);


   Qt3DExtras::QFirstPersonCameraController *cam_controller = new Qt3DExtras::QFirstPersonCameraController(root_entity);
   cam_controller->setCamera(camera_entity);

   Qt3DCore::QEntity *sphere_entity = new Qt3DCore::QEntity(root_entity);
   Qt3DExtras::QSphereMesh *sphere_mesh = new Qt3DExtras::QSphereMesh();
   sphere_mesh->setRings(30);
   sphere_mesh->setSlices(30);
   sphere_mesh->setRadius(3);

   Qt3DCore::QTransform *sphere_transform = new Qt3DCore::QTransform();
   sphere_transform->setScale(1.2f);
   sphere_transform->setTranslation(QVector3D(0.0f, 0.0f, 1.0f));

   Qt3DExtras::QPhongMaterial *sphere_material = new Qt3DExtras::QPhongMaterial();
   sphere_material->setDiffuse(QColor(QRgb(0xa69929)));


   sphere_entity->addComponent(sphere_mesh);
   sphere_entity->addComponent(sphere_material);
   sphere_entity->addComponent(sphere_transform);
   sphere_entity->setEnabled(true);

   view->setRootEntity(root_entity);

    container->show();
    container->resize(1200, 800);

    return a.exec();
}