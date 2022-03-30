//
// Created by shenhangke on 2022-3-30.
//
#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>


int main(int args,char** argu){
    osgViewer::Viewer view;
    view.apply(new osgViewer::SingleWindow(200,200,800,600));

    {
        //test osg
        osg::ref_ptr<osg::Geometry> geo=new osg::Geometry;
        osg::ref_ptr<osg::Vec3Array> vertexs=new osg::Vec3Array;
        osg::ref_ptr<osg::Vec4Array> colors=new osg::Vec4Array;
        vertexs->push_back(osg::Vec3(0,0.5,0));
        vertexs->push_back(osg::Vec3(0,0.5,0));

    }

    view.run();
}