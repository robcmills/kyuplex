
//#include "kx.h"
#include "kxMesh.h"
#include "kxKillCube.h"
#include "kxPlayerCube.h"
#include "kxQuadPlane.h"
#include "kxSpline.h"
#include "kxCollectable.h"
#include "kxBox.h"
#include "kxLevel.h"
#include "kxLight.h"
#include "kxEnums.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



void kx::ResetScene()
{
    /*
    std::cout<< "ResetScene()" << std::endl;

    playerCube->reset(); 
    //activeLevel->reset(); 

    smgr->clear();     // IRRLICHT
    ClearPhysics();    // BULLET

    // camPivot
    // Acts as pivot point parent for cam
    camPivot = smgr->addEmptySceneNode( smgr->getRootSceneNode(), KNT_CAM_PIVOT );
    camPivot->setName( "camPivot" );
    camPivot->setPosition( playerCube->getPosition() );

    // playerCamera
    camDistance= 12.8f;
    playerCam= smgr->addCameraSceneNode( camPivot, vector3df( 0,0,camDistance ), camPivot->getPosition(), KNT_CAMERA );
    playerCam->setName( "playerCamera" );
    camPivot->setRotation( vector3df( -9.6f,45,0 ));
    //playerCam->setFOV (100); 


    //showPhysicsWireframes= false;
    controlType = ECT_IMPULSE; 
    controlNodeID = KNT_PLAYER_CUBE; 
    pControlNode= playerCube;


    gameState= EGS_GAME;
    isGUIvisible= false; 
    isFirstGUI= false;
    cursor->setPosition( screenCenter );

    */
}
   

bool kx::SaveLevel( EKX_LEVEL_FORMAT lf ) // {{{1
{
    return false;
} // }}}


bool kx::LoadLevel()
{
    std::cout<< "LoadLevel()" << std::endl;

    fileSys->changeWorkingDirectoryTo( kxBaseDir );

    //ResetScene();

    /*
    if( !LoadBsp( "./bsp/kxMap1.bsp" ))
    {
        std::cout<< "Error: couldn't load bsp" << std::endl;
        // TODO: load default scene here?
        return false;
    }
    */

    LoadEntities();

    return true;
}

void kx::LoadEntities()
{
    path entFile= kxBaseDir;
    entFile += "/entities/";
    entFile += activeLevel->num * 100 + activeLevel->progress;
    entFile += ".xml";

    std::cout<< entFile.c_str() <<std::endl;

    IXMLReader* reader = fileSys->createXMLReader( entFile );
    if( !reader )
    {
        std::cout<< "Error: couldn't create XML reader" <<std::endl;
        return;
    }

    while( reader->read() )
    {
        switch( reader->getNodeType() )
        {
        case EXN_ELEMENT:
            if( stringw( L"kxCollectable" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                kxCollectable* c= new kxCollectable();
                c->setName( attr->getAttributeAsString( "name" ));
                c->setPosition( attr->getAttributeAsVector3d( "position" ));
                c->setRotation( attr->getAttributeAsVector3d( "rotation" ));

                attr->drop();
                activeLevel->collectables.push_back( c ); 
                break;
            }

            else if( stringw( L"kxSpline" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                bool isOpen= attr->getAttributeAsBool( "isOpen" );
                SColorf wireColor= attr->getAttributeAsColorf( "wirecolor" );

                array< S3DVertex > vertices;
                S3DVertex vx;
                vx.Color= wireColor.toSColor();
                for( u32 i=0; i<attr->getAttributeCount(); i++ )
                {
                    if( attr->getAttributeType(i) != EAT_VECTOR3D ) continue;
                    vx.Pos= attr->getAttributeAsVector3d(i);
                    vertices.push_back( vx );
                }
                // splineMaterial
                SMaterial sMat; // default white mat; wirecolor is set with vertex colors
                sMat.Lighting= false;

                kxSpline* sNode= new kxSpline( vertices, isOpen, sMat );
                sNode->setName( attr->getAttributeAsString( "name" ));
                activeLevel->splineArray.push_back( sNode );
                attr->drop();
                break;
            }

            else if( stringw( L"kxLight" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                kxLight* l= new kxLight();
                l->setName( attr->getAttributeAsString( "name" ));
                //l->setPosition( (const btVector3&) attr->getAttributeAsVector3d( "position" ));
                l->setPosition( (const btVector3&) attr->getAttributeAsVector3d( "position" ));
                l->lightNode->setRadius( attr->getAttributeAsFloat( "radius" ));
                //SLight& data= l->lightNode->getLightData();
                //data.Attenuation= vector3df( 0,.1f,0 );

                activeLevel->lights.push_back( l );
                attr->drop();
                break;
            }

            else if( stringw( L"kxBox" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                kxBox* bx= new kxBox(); 
                bx->setSize( attr->getAttributeAsVector3d( "size" ));
                bx->setPosition( attr->getAttributeAsVector3d( "position" ));
                bx->setRotation( attr->getAttributeAsVector3d( "rotation" ));

                bx->setColor( attr->getAttributeAsColorf( "diffuse" ).toSColor() );
                bx->setEdgeColor( attr->getAttributeAsColorf( "edgeColor" ).toSColor() );

                bx->isCollisionObject= attr->getAttributeAsBool( "isCollisionObject" );
                bx->isRigidBody= attr->getAttributeAsBool( "isRigidBody" );

                activeLevel->boxes.push_back( bx );
                attr->drop();
                break;
            }

            else if( stringw( L"kxRigidCube" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                kxFractureCube* fc= new kxFractureCube(); 
                fc->setSize( attr->getAttributeAsVector3d( "size" ));
                fc->setPosition( (const btVector3&) attr->getAttributeAsVector3d( "position" ));
                fc->setRotation( attr->getAttributeAsVector3d( "rotation" ));

                activeLevel->fractureCubes.push_back( fc );
                attr->drop();
                break;
            }

            else if( stringw( L"kxCollisionCube" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                btVector3 halfExtents= (const btVector3&) attr->getAttributeAsVector3d( "size" );
                halfExtents /= btScalar( 2 );
                btCollisionShape* shape = new btBoxShape( halfExtents );

                btCollisionObject* co = new btCollisionObject();
                co->setCollisionShape( shape );
                co->getWorldTransform().setOrigin( (const btVector3&) attr->getAttributeAsVector3d( "position" ));
                //co->getWorldTransform().setRotationDegrees( (const btVector3&) attr->getAttributeAsVector3d( "rotation" ));
                         //addCollisionObject( co, COLLISION_GROUP, COLLISION_MASK )
                physics->addCollisionObject( co, ECG_STATIC, ECG_FRACTURE_CUBE | ECG_LIGHT );
                // TODO: add collision group and mask to userProps 
                attr->drop();
                break;
            }

            else if( stringw( L"kxMesh" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                std::cout<< "Loading mesh: "<< attr->getAttributeAsString( "mesh" ).c_str()<< std::endl;
                IAnimatedMesh* mesh= smgr->getMesh( attr->getAttributeAsString( "mesh" ));
                if( !mesh ) break;
                IAnimatedMeshSceneNode* meshNode= smgr->addAnimatedMeshSceneNode( mesh );
                meshNode->addShadowVolumeSceneNode(); 
                smgr->setShadowColor( SColor(150,10,10,10) );
                //shadow->setShadowMesh( mesh->getMesh(0) );
                //shadow->updateShadowVolumes();
                //mesh->getMeshBuffer(0)->recalculateBoundingBox();
                //kxMesh* meshNode= new kxMesh( mesh );

                    //attr->getAttributeAsVector3d( "position" ));
                    //attr->getAttributeAsVector3d( "rotation" ));


                //dimension2du resolution= attr->getAttributeAsDimension2d( "resolution" );
                //stringc pathToDiffuseTex= attr->getAttributeAsString( "pathToDiffuseTex" );
                //meshNode->getMaterial(0).setTexture( 0, driver->addTexture( resolution, pathToDiffuseTex.c_str() ));
                //meshNode->setDebugDataVisible( EDS_NORMALS );
                // we are baking lighting into diffuse texture
                //meshNode->getMaterial(0).Lighting= false;
                // TODO: set wire mat props here
                //meshNode->setDebugDataVisible( EDS_MESH_WIRE_OVERLAY );

                activeLevel->meshArray.push_back( meshNode );
                attr->drop();
                break;
            }

            else if( stringw( L"kxSkyBox" ) == reader->getNodeName() )
            {
                IAttributes* attr = fileSys->createEmptyAttributes( driver );
                attr->read( reader );

                ITexture* top= driver->getTexture( attr->getAttributeAsString( "top" ));
                //activeLevel->runTimeTex.push_back( top ); // TODO: memory leak??
                ITexture* bottom= driver->getTexture( attr->getAttributeAsString( "bottom" ));
                ITexture* left= driver->getTexture( attr->getAttributeAsString( "left" ));
                ITexture* right= driver->getTexture( attr->getAttributeAsString( "right" ));
                ITexture* front= driver->getTexture( attr->getAttributeAsString( "front" ));
                ITexture* back= driver->getTexture( attr->getAttributeAsString( "back" ));
                ISceneNode* skyboxNode= smgr->addSkyBoxSceneNode( top,bottom,left,right,front,back );
                //activeLevel->skyBox= skyboxNode;
                attr->drop();
                break;
            }
        }
    }
    reader->drop();
    fileSys->changeWorkingDirectoryTo( kxBaseDir );
}

