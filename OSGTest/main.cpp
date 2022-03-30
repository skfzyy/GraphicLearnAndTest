//
// Created by shenhangke on 2022-3-30.
//
#include <osg/Group>
#include <osg/Program>
#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>

#define ShaderDir CURRENT_DIR "/Shader/"
#define GLSL330(src) "#version 330\n" #src

const char* vertSource =
    GLSL330(layout(location = 0) in vec3 vertex; layout(location = 1) in vec4 inColor;

            out vec4 color;

            void main() {
                color = inColor;
                gl_Position = vec4(vertex, 1.0);
            });

const char* fragSource = GLSL330(in vec4 color; out vec4 fragColor;

                                 void main() { fragColor = color; });

int main(int args, char** argu)
{
    osgViewer::Viewer view;
    view.apply(new osgViewer::SingleWindow(200, 200, 800, 600));
    osg::ref_ptr<osg::Group> root = new osg::Group;
    view.setSceneData(root);
    {
        // test osg
        osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
        osg::ref_ptr<osg::Vec3Array> vertexs = new osg::Vec3Array;
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
        vertexs->push_back(osg::Vec3(0, 0.5, 0));
        vertexs->push_back(osg::Vec3(0.5, 0, 0));
        vertexs->push_back(osg::Vec3(0, 0, 0));
        colors->push_back(osg::Vec4d(1.0, 0, 0, 1.0));
        geo->setVertexAttribArray(0, vertexs, osg::Array::BIND_PER_VERTEX);
        // geo->setColorArray(colors, osg::Array::BIND_OVERALL);
        geo->setVertexAttribArray(1, colors, osg::Array::BIND_OVERALL);
        geo->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 3));
        geo->setUseVertexBufferObjects(true);
        geo->setUseVertexArrayObject(true);
        geo->setUseDisplayList(false);
        osg::ref_ptr<osg::Program> program = new osg::Program;
        osg::ref_ptr<osg::Shader> vertexShader = new osg::Shader(osg::Shader::VERTEX);
         bool ret = vertexShader->loadShaderSourceFromFile(std::string(ShaderDir) +
                                                           std::string("test.vert"));

        osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
         ret =
             fragShader->loadShaderSourceFromFile(std::string(ShaderDir) +
             std::string("test.frag"));

        program->addShader(vertexShader);
        program->addShader(fragShader);
        geo->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON);
        root->addChild(geo);
    }

    view.run();
}