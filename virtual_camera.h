//
// Created by 金宇超 on 15/11/12.
//

#ifndef OSGVIDEO3D_VIRTUAL_CAMERA_H
#define OSGVIDEO3D_VIRTUAL_CAMERA_H

#include <opencv2/opencv.hpp>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

class virtual_camera {
private:
    osg::Camera* cam;
public:
    static osg::Geode* createPyramid() {
        osg::Geode* pyramidGeode = new osg::Geode();
        osg::Geometry* pyramidGeometry = new osg::Geometry();

        //Associate the pyramid geometry with the pyramid geode
        //   Add the pyramid geode to the root node of the scene graph.

        pyramidGeode->addDrawable(pyramidGeometry);

        //Declare an array of vertices. Each vertex will be represented by
        //a triple -- an instances of the vec3 class. An instance of
        //osg::Vec3Array can be used to store these triples. Since
        //osg::Vec3Array is derived from the STL vector class, we can use the
        //push_back method to add array elements. Push back adds elements to
        //the end of the vector, thus the index of first element entered is
        //zero, the second entries index is 1, etc.
        //Using a right-handed coordinate system with 'z' up, array
        //elements zero..four below represent the 5 points required to create
        //a simple pyramid.

        osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
        pyramidVertices->push_back( osg::Vec3(-50,-50,  0) ); // front left
        pyramidVertices->push_back( osg::Vec3( 50,-50,  0) ); // front right
        pyramidVertices->push_back( osg::Vec3( 50, 50,  0) ); // back right
        pyramidVertices->push_back( osg::Vec3(-50, 50,  0) ); // back left
        pyramidVertices->push_back( osg::Vec3(  0,  0,-50) ); // peak

        //Associate this set of vertices with the geometry associated with the
        //geode we added to the scene.

        pyramidGeometry->setVertexArray( pyramidVertices );

        //Next, create a primitive set and add it to the pyramid geometry.
        //Use the first four points of the pyramid to define the base using an
        //instance of the DrawElementsUint class. Again this class is derived
        //from the STL vector, so the push_back method will add elements in
        //sequential order. To ensure proper backface cullling, vertices
        //should be specified in counterclockwise order. The arguments for the
        //constructor are the enumerated type for the primitive
        //(same as the OpenGL primitive enumerated types), and the index in
        //the vertex array to start from.

        osg::DrawElementsUInt* pyramidBase =
                new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
        pyramidBase->push_back(3);
        pyramidBase->push_back(2);
        pyramidBase->push_back(1);
        pyramidBase->push_back(0);
        pyramidGeometry->addPrimitiveSet(pyramidBase);

        //Repeat the same for each of the four sides. Again, vertices are
        //specified in counter-clockwise order.

        osg::DrawElementsUInt* pyramidFaceOne =
                new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
        pyramidFaceOne->push_back(0);
        pyramidFaceOne->push_back(1);
        pyramidFaceOne->push_back(4);
        pyramidGeometry->addPrimitiveSet(pyramidFaceOne);

        osg::DrawElementsUInt* pyramidFaceTwo =
                new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
        pyramidFaceTwo->push_back(1);
        pyramidFaceTwo->push_back(2);
        pyramidFaceTwo->push_back(4);
        pyramidGeometry->addPrimitiveSet(pyramidFaceTwo);

        osg::DrawElementsUInt* pyramidFaceThree =
                new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
        pyramidFaceThree->push_back(2);
        pyramidFaceThree->push_back(3);
        pyramidFaceThree->push_back(4);
        pyramidGeometry->addPrimitiveSet(pyramidFaceThree);

        osg::DrawElementsUInt* pyramidFaceFour =
                new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
        pyramidFaceFour->push_back(3);
        pyramidFaceFour->push_back(0);
        pyramidFaceFour->push_back(4);
        pyramidGeometry->addPrimitiveSet(pyramidFaceFour);

        //Declare and load an array of Vec4 elements to store colors.

        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 0 red
        colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); //index 1 green
        colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); //index 2 blue
        colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 3 white
        colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 4 red

        //The next step is to associate the array of colors with the geometry,
        //assign the color indices created above to the geometry and set the
        //binding mode to _PER_VERTEX.

        pyramidGeometry->setColorArray(colors);
        pyramidGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

        return pyramidGeode;
    }
    osg::Matrixd matrixd;
    osg::Geode* pyramid;
    osg::PositionAttitudeTransform* node;
    virtual_camera(const cv::FileNodeIterator it) {
        cam = new osg::Camera();
        cam->setName((*it)["name"]);
        cam->addDescription((*it)["desc"]);
        cv::Mat mat;
        (*it)["Matrix"] >> mat;
        matrixd = osg::Matrixd((double*)mat.data);
        node = new osg::PositionAttitudeTransform;
        node->setPosition(matrixd.getTrans());
        node->setAttitude(matrixd.getRotate());
        pyramid = createPyramid();
        node->addChild(pyramid);
    }

    void setMatrix(osg::Matrixd mat) {
        this->matrixd = mat;
        node->setPosition(matrixd.getTrans());
        node->setAttitude(matrixd.getRotate());
    }
    const osg::Camera* getCamera() { return cam; }
};

#endif //OSGVIDEO3D_VIRTUAL_CAMERA_H