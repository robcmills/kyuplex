#include "kx.h"
#include "kxBspConverter.h"
#include "kxBspLoader.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


bool kx::LoadBsp( const path& map )
{
    // loads q3 style bsp map into irr node 
    // and creates rigid bodies from level geometry

	//device->getFileSystem()->addZipFileArchive("./maps/pak0.pk3");

	IQ3LevelMesh* const mesh = (IQ3LevelMesh*) smgr->getMesh( map.c_str() );

	//IAnimatedMesh* mesh= 0;
    //mesh = smgr->getMesh( map.c_str() ); 
	if( !mesh )
    {   
        std::cout<< "Couldn't create level mesh" << std::endl;
        return false;
    }

	ISceneNode* node= 0;
    IMesh* const geometry = mesh->getMesh( quake3::E_Q3_MESH_GEOMETRY );
    node = smgr->addOctreeSceneNode( geometry, 0, -1, 4096 );


    // level01 IRRLICHT SCENE NODE
    //ISceneNode* node = smgr->addOctreeSceneNode( mesh->getMesh(0), 0, -1, 1024 );
    node->setName( L"bspOctreeNode" );
    node->setID( KNT_LEVEL );
    //node->setMaterialFlag( EMF_LIGHTING, true );
    node->setScale( vector3df( .1f ));  // scale down to Bullet range .05-10
    //node->setPosition (vector3df (-256, -64, -256));
    //node->setMaterialFlag (EMF_BACK_FACE_CULLING, true);
    //node->setMaterialTexture (0, grayCheckers);

    /*
    const IMesh* const additional_mesh = mesh->getMesh( quake3::E_Q3_MESH_ITEMS );
    for ( u32 i = 0; i!= additional_mesh->getMeshBufferCount(); ++i )
    {
        const IMeshBuffer* meshBuffer = additional_mesh->getMeshBuffer(i);
        const video::SMaterial& material = meshBuffer->getMaterial();

        // The ShaderIndex is stored in the material parameter
        const s32 shaderIndex = (s32) material.MaterialTypeParam2;

        // the meshbuffer can be rendered without additional support, or it has no shader
        const quake3::IShader *shader = mesh->getShader(shaderIndex);
        if (0 == shader)
        {
            continue;
        }
        std::cout<< shader->name.c_str() <<std::endl;

        // we can dump the shader to the console in its
        // original but already parsed layout in a pretty
        // printers way.. commented out, because the console
        // would be full...
        // quake3::dumpShader ( Shader );

        node = smgr->addQuake3SceneNode(meshBuffer, shader);
    }

	quake3::tQ3EntityList &entityList = mesh->getEntityList();

    quake3::IEntity search;
    search.name= "light";

    for( u32 i=0; i<entityList.size(); i++ )
    {
        //for( u32 j=0; j<entityList[i].VarGroup.size(); j++ )
        std::cout<< entityList[i].name.c_str() <<std::endl;
        std::cout<< entityList[i].VarGroup->VariableGroup.size() <<std::endl;
    }

    s32 index = entityList.binary_search( search );
    if( index >= 0 )
    {
        std::cout<< "found a light; index=" << index <<std::endl;
        s32 notEndList;
        do
        {
            const quake3::SVarGroup *group = entityList[index].getGroup(1);

            u32 parsepos = 0;
            const core::vector3df pos =
                quake3::getAsVector3df(group->get("origin"), parsepos);

            parsepos = 0;
            const f32 angle = quake3::getAsFloat(group->get("angle"), parsepos);

            core::vector3df target(0.f, 0.f, 1.f);
            target.rotateXZBy(angle);

            camera->setPosition(pos);
            camera->setTarget(pos + target);

            ++index;
            notEndList = (	index < (s32) entityList.size () &&
                            entityList[index].name == search.name &&
                            (device->getTimer()->getRealTime() >> 3 ) & 1
                        );
            //notEndList = index == 2;
        //} while ( notEndList );
    }
*/

    // BSP -> BULLET OBJECT
    io::IReadFile* file= device->getFileSystem()->createAndOpenFile( map.c_str() ); 
    long int bspsize= file->getSize();
    void* bspbuffer= new char [bspsize];
    file->read( bspbuffer,bspsize );
    BspConverter* convert= new BspConverter();
    float scaling= .1f;
    BspLoader bspLoader;
    bspLoader.loadBSPFile( bspbuffer );
    convert->convertBsp( bspLoader, scaling );

    // TODO: convertBsp just adds rigid bodies to world, 
    // should we change that so we can access those rb's?
    return true;
}

