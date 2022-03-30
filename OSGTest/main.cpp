//
// Created by shenhangke on 2022-3-30.
//
#include <osg/Camera>
#include <osg/Group>
#include <osg/Program>
#include <osg/Texture2D>
#include <osg/Uniform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>

#define ShaderDir CURRENT_DIR "/Shader/"
#define ResourceDir CURRENT_DIR "/Resource/"
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

void UseVAO(osg::Drawable* geo)
{
    geo->setUseVertexBufferObjects(true);
    geo->setUseVertexArrayObject(true);
    geo->setUseDisplayList(false);
}

osg::ref_ptr<osg::Program> CreateProgram(std::string vertShader, std::string fragShader)
{
    osg::ref_ptr<osg::Program> program = new osg::Program;
    osg::ref_ptr<osg::Shader> vert = new osg::Shader(osg::Shader::VERTEX);
    vert->loadShaderSourceFromFile(std::string(ShaderDir) + vertShader);
    osg::ref_ptr<osg::Shader> frag = new osg::Shader(osg::Shader::FRAGMENT);
    frag->loadShaderSourceFromFile(std::string(ShaderDir) + fragShader);
    program->addShader(vert);
    program->addShader(frag);
    return program;
}

osg::ref_ptr<osg::Texture2D> CreateTexutre(std::string textureName)
{
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(std::string(ResourceDir) + textureName);
    texture->setImage(image);
    return texture;
}

osg::ref_ptr<osg::Geometry> CreateScreenBack()
{
    //没有设置texture
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> vertexarr = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec2Array> texarr = new osg::Vec2Array;
    geometry->setVertexAttribArray(0, vertexarr, osg::Array::BIND_PER_VERTEX);
    vertexarr->push_back(osg::Vec3(-1.0, -1.0, 0.0));
    vertexarr->push_back(osg::Vec3(1.0, -1.0, 0.0));
    vertexarr->push_back(osg::Vec3(1.0, 1.0, 0.0));
    vertexarr->push_back(osg::Vec3(-1.0, -1.0, 0.0));
    vertexarr->push_back(osg::Vec3(1.0, 1.0, 0.0));
    vertexarr->push_back(osg::Vec3(-1.0, 1.0, 0.0));
    texarr->push_back(osg::Vec2(0.0, 0.0));
    texarr->push_back(osg::Vec2(1.0, 0.0));
    texarr->push_back(osg::Vec2(1.0, 1.0));
    texarr->push_back(osg::Vec2(0.0, 0.0));
    texarr->push_back(osg::Vec2(1.0, 1.0));
    texarr->push_back(osg::Vec2(0.0, 1.0));
    geometry->setVertexAttribArray(1, texarr, osg::Array::BIND_PER_VERTEX);

    osg::ref_ptr<osg::PrimitiveSet> primitiveSet =
        new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 6);
    geometry->addPrimitiveSet(primitiveSet);

    UseVAO(geometry);
    osg::ref_ptr<osg::Program> program = new osg::Program;
    osg::ref_ptr<osg::Shader> vert = new osg::Shader(osg::Shader::VERTEX);
    vert->loadShaderSourceFromFile(std::string(ShaderDir) + std::string("backGeo.vert"));
    osg::ref_ptr<osg::Shader> frag = new osg::Shader(osg::Shader::FRAGMENT);
    frag->loadShaderSourceFromFile(std::string(ShaderDir) + std::string("backGeo.frag"));
    program->addShader(vert);
    program->addShader(frag);

    osg::StateSet* stateSet = geometry->getOrCreateStateSet();
    stateSet->setAttributeAndModes(program, osg::StateAttribute::ON |
                                                osg::StateAttribute::OVERRIDE |
                                                osg::StateAttribute::PROTECTED);
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    return geometry;
}

int main(int args, char** argu)
{
    osgViewer::Viewer view;
    view.apply(new osgViewer::SingleWindow(200, 200, 800, 600));
    osg::ref_ptr<osg::Group> root = new osg::Group;
    view.setSceneData(root);
    osg::ref_ptr<osg::Texture2D> colorTexture = new osg::Texture2D;
    osg::ref_ptr<osg::Texture2D> depthStencilTexture = new osg::Texture2D;
    {
        // init texture
        colorTexture->setTextureSize(800, 600);
        colorTexture->setInternalFormat(GL_RGBA);
        colorTexture->setSourceFormat(GL_RGBA);
        colorTexture->setSourceType(GL_FLOAT);
        colorTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        colorTexture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

        depthStencilTexture->setTextureSize(800, 600);
        depthStencilTexture->setInternalFormat(GL_DEPTH24_STENCIL8_EXT);
        depthStencilTexture->setSourceFormat(GL_DEPTH_STENCIL_EXT);
        depthStencilTexture->setSourceType(GL_UNSIGNED_INT_24_8_EXT);
    }

    {
        //首先渲染地板?
        //地板是渲染到colorTexuture上的
        osg::ref_ptr<osg::Camera> cameraFloor = new osg::Camera;
        cameraFloor->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                                  GL_STENCIL_BUFFER_BIT);
        cameraFloor->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
        cameraFloor->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        cameraFloor->setRenderOrder(osg::Camera::PRE_RENDER, 0);
        cameraFloor->setViewport(0, 0, 800, 600);
        cameraFloor->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        cameraFloor->attach(osg::Camera::COLOR_BUFFER0, colorTexture);
        cameraFloor->attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, depthStencilTexture);
        cameraFloor->getOrCreateStateSet()->setTextureAttribute(0, colorTexture,
                                                                osg::StateAttribute::ON |
                                                                    osg::StateAttribute::OVERRIDE |
                                                                    osg::StateAttribute::PROTECTED);
        osg::ref_ptr<osg::Program> program = new osg::Program;
        osg::ref_ptr<osg::Shader> vert = new osg::Shader(osg::Shader::VERTEX);
        vert->loadShaderSourceFromFile(std::string(ShaderDir) + std::string("backGeo.vert"));
        osg::ref_ptr<osg::Shader> frag = new osg::Shader(osg::Shader::FRAGMENT);
        frag->loadShaderSourceFromFile(std::string(ShaderDir) + std::string("backGeo.frag"));
        program->addShader(vert);
        program->addShader(frag);
        cameraFloor->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON);

        auto createFloor = []() -> osg::ref_ptr<osg::Geometry> {
            osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
            osg::ref_ptr<osg::Vec3Array> vertexarr = new osg::Vec3Array;
            osg::ref_ptr<osg::Vec2Array> texarr = new osg::Vec2Array;
            geometry->setVertexAttribArray(0, vertexarr, osg::Array::BIND_PER_VERTEX);
            vertexarr->push_back(osg::Vec3(5.0f, -0.5f, 5.0f));
            vertexarr->push_back(osg::Vec3(-5.0f, -0.5f, 5.0f));
            vertexarr->push_back(osg::Vec3(-5.0f, -0.5f, -5.0f));
            vertexarr->push_back(osg::Vec3(5.0f, -0.5f, 5.0f));
            vertexarr->push_back(osg::Vec3(-5.0f, -0.5f, -5.0f));
            vertexarr->push_back(osg::Vec3(5.0f, -0.5f, -5.0f));
            texarr->push_back(osg::Vec2(2.0f, 0.0f));
            texarr->push_back(osg::Vec2(0.0f, 0.0f));
            texarr->push_back(osg::Vec2(0.0f, 2.0f));
            texarr->push_back(osg::Vec2(2.0f, 0.0f));
            texarr->push_back(osg::Vec2(0.0f, 2.0f));
            texarr->push_back(osg::Vec2(2.0f, 2.0f));
            geometry->setVertexAttribArray(1, texarr, osg::Array::BIND_PER_VERTEX);
            osg::ref_ptr<osg::PrimitiveSet> primitiveSet =
                new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 6);
            geometry->addPrimitiveSet(primitiveSet);
            UseVAO(geometry);

            //设置材质
            auto floorTexture = CreateTexutre("metal.png");
            geometry->getOrCreateStateSet()->setTextureAttribute(
                0, floorTexture, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
            geometry->getOrCreateStateSet()->addUniform(new osg::Uniform("colorTexture", 0));
            return geometry;
        };
        cameraFloor->addChild(createFloor());
        root->addChild(cameraFloor);
    }

    {
        //绘制outline在第二个相机上,其中的第一个相机用来绘制第一次的stencil,第二个相机用来绘制实际的标线
        osg::ref_ptr<osg::Camera> outLine1 = new osg::Camera;
        osg::ref_ptr<osg::Camera> outLine2 = new osg::Camera;
    }

    {
        //绘制texture在背景布上
        auto geometryBack = CreateScreenBack();
        geometryBack->getOrCreateStateSet()->setTextureAttribute(
            0, colorTexture, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
        geometryBack->getOrCreateStateSet()->addUniform(new osg::Uniform("colorTexture", 0));
        root->addChild(geometryBack);
    }

    view.run();
}