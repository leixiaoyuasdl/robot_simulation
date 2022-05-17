#-------------------------------------------------
#
# Project created by QtCreator 2020-06-18T22:38:47
#
#-------------------------------------------------

QT       += core gui opengl xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobMach
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    animation.cpp \
    coordinate_system.cpp \
    information.cpp \
    joint_angle.cpp \
        main.cpp \
    modelxml.cpp \
    solverpos.cpp \
    transfer_station.cpp \
        view.cpp \
    modelview.cpp \
    modeltree.cpp

HEADERS += \
    animation.h \
    coordinate_system.h \
    information.h \
    joint_angle.h \
    modelxml.h \
    solverpos.h \
    transfer_station.h \
        view.h \
    modelview.h \
    modeltree.h

FORMS += \
    information.ui \
    joint_angle.ui \
        view.ui \
    modeltree.ui
INCLUDEPATH +=  D:\opencascade\OpenCASCADE-7.4.0-vc14-64\opencascade-7.4.0\inc
INCLUDEPATH +=  E:\OpenCASCADE-7.4.0-vc14-64\opencascade-7.4.0\inc

DEFINES += WNT

#LIBS += -LD:\opencascade\OpenCASCADE-7.4.0-vc14-64\opencascade-7.4.0\win64\vc14\lib \
#        -lTKernel -lTKMath -lTKV3d -lTKOpenGl \
#        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
#        -lTKSTEPBase -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
#        -lTKXSBase -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
#        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset -lTKLCAF -lTKService -lTKXMesh -lTKXSDRAW -lTKMeshVS -lTKQADraw -lTKRWMesh \

LIBS += -LE:\OpenCASCADE-7.4.0-vc14-64\opencascade-7.4.0\win64\vc14\lib \
        -lTKernel -lTKMath -lTKV3d -lTKOpenGl \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset -lTKLCAF -lTKService -lTKXMesh -lTKXSDRAW -lTKMeshVS -lTKQADraw -lTKRWMesh \

INCLUDEPATH += D:\github\eigen-3.3.7
INCLUDEPATH += D:\orocos_kdl\orocos_kinematics_dynamics-master\orocos_kdl\eigen3
INCLUDEPATH += D:\github\orocos_kinematics_dynamics\orocos_kdl\src
INCLUDEPATH += D:\orocos_kdl\orocos_kinematics_dynamics-master\orocos_kdl\src
#LIBS += -LD:\github\orocos_kinematics_dynamics\orocos_kdl\build\src\Debug\
#        -lorocos-kdld\
LIBS += -LD:\orocos_kdl\orocos_kinematics_dynamics-master\orocos_kdl\build\src\Debug \
        -lorocos-kdld \
