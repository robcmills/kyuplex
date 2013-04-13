
#ifndef BSP_CONVERTER_H
#define BSP_CONVERTER_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedObjectArray.h>
#include <LinearMath/btGeometryUtil.h>
#include <stdio.h>
#include <string.h>

#include "kx.h"
#include "kxBspLoader.h"


///BspConverter turns a loaded bsp level into convex parts (vertices)
class BspConverter
{
    private:
        kx& k;

	public:
        BspConverter () : k (kx::getInstance()) {}
		virtual ~BspConverter() {}

		void convertBsp (BspLoader& bspLoader, float scaling);

		///this callback is called for each brush that succesfully converted into vertices
		void addConvexVerticesCollider (btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation);

};

void BspConverter::addConvexVerticesCollider (btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation)
{
    ///perhaps we can do something special with entities (isEntity)
    ///like adding a collision Triggering (as example)
    
    if (vertices.size() > 0)
    {
        ///irrlicht swaps Z and Y in a BSP file?
        btAlignedObjectArray <btVector3> rotatedVerts;
        for (int i=0;i<vertices.size();i++)
        {
            rotatedVerts.push_back (btVector3 (vertices[i].getX(), vertices[i].getZ(), vertices[i].getY())); //* k.physicsWorldScaling);
        }
        float mass = 0.f;
        btTransform startTransform;
        //can use a shift
        startTransform.setIdentity();
        startTransform.setOrigin (btVector3(0,0,-10.f));
        //this create an internal copy of the vertices
        
        btCollisionShape* shape = new btConvexHullShape (&(rotatedVerts[0].getX()), rotatedVerts.size());
        k.collisionShapes.push_back (shape);

        btRigidBody* body = new btRigidBody (0.f, 0, shape);
        body->setCenterOfMassTransform (btTransform (btQuaternion::getIdentity(), btVector3(0,0,0)));  //btVector3(-256, -64, -256)));
        body->setRestitution (1);
        //body->setUserPointer ((void*)k.activeLevel);
        k.physics->addRigidBody (body, ECG_STATIC, ECG_PLAYER_CUBE | ECG_FRACTURE_CUBE);
    }
}


void BspConverter::convertBsp (BspLoader& bspLoader,float scaling)
{
    
    float	playstartf[3] = {0,0,100};

    if (bspLoader.findVectorByName(&playstartf[0],"info_player_start"))
    {
        printf("found playerstart\n");
    } 
    else
    {
        if (bspLoader.findVectorByName(&playstartf[0],"info_player_deathmatch"))
        {
            printf("found deatchmatch start\n");
        }
    }

    btVector3 playerStart (playstartf[0],playstartf[1],playstartf[2]);


    playerStart[2] += 20.f; //start a bit higher

    playerStart *= scaling;

    

    //progressBegin("Loading bsp");

    for (int i=0;i<bspLoader.m_numleafs;i++)
    {
        printf("Reading bspLeaf %i from total %i (%f procent)\n",i, bspLoader.m_numleafs,(100.f*(float)i/float(bspLoader.m_numleafs)) );
        
        bool isValidBrush = false;
        
        BSPLeaf&	leaf = bspLoader.m_dleafs[i];

        for (int b=0;b<leaf.numLeafBrushes;b++)
        {
            btAlignedObjectArray<btVector3> planeEquations;
            
            int brushid = bspLoader.m_dleafbrushes[leaf.firstLeafBrush+b];

            BSPBrush& brush = bspLoader.m_dbrushes[brushid];
            if (brush.shaderNum!=-1)
            {
                if (bspLoader.m_dshaders[ brush.shaderNum ].contentFlags & BSPCONTENTS_SOLID)
                {
                    brush.shaderNum = -1;

                    for (int p=0;p<brush.numSides;p++)
                    {
                        int sideid = brush.firstSide+p;
                        BSPBrushSide& brushside = bspLoader.m_dbrushsides[sideid];
                        int planeid = brushside.planeNum;
                        BSPPlane& plane = bspLoader.m_dplanes[planeid];
                        btVector3 planeEq;
                        planeEq.setValue(
                            plane.normal[0],
                            plane.normal[1],
                            plane.normal[2]);
                        planeEq[3] = scaling*-plane.dist;

                        planeEquations.push_back(planeEq);
                        isValidBrush=true;
                    }
                    if (isValidBrush)
                    {

                        btAlignedObjectArray<btVector3>	vertices;
                        btGeometryUtil::getVerticesFromPlaneEquations(planeEquations,vertices);
                    
                        bool isEntity = false;
                        btVector3 entityTarget(0.f,0.f,0.f);
                        addConvexVerticesCollider(vertices,isEntity,entityTarget);
                    
                    }
                }
            } 
        }
    }

#define USE_ENTITIES
#ifdef USE_ENTITIES
		
{
    int i;
    for (i=0;i<bspLoader.m_num_entities;i++)
        {
            const BSPEntity& entity = bspLoader.m_entities[i];
            const char* cl = bspLoader.getValueForKey(&entity,"classname");
            if ( !strcmp( cl, "trigger_push" ) ) {
                btVector3 targetLocation(0.f,0.f,0.f);

                cl = bspLoader.getValueForKey(&entity,"target");
                if ( strcmp( cl, "" ) ) {
                    //its not empty so ...

                    /*
                    //lookup the target position for the jumppad:
                    const BSPEntity* targetentity = bspLoader.getEntityByValue( "targetname" , cl );
                    if (targetentity)
                    {
                        if (bspLoader.getVectorForKey( targetentity , "origin",&targetLocation[0]))
                        {
                                                            
                        }
                    }
                    */


                    cl = bspLoader.getValueForKey(&entity,"model");
                    if ( strcmp( cl, "" ) ) {
                        // add the model as a brush
                        if (cl[0] == '*')
                        {
                            int modelnr = atoi(&cl[1]);
                            if ((modelnr >=0) && (modelnr < bspLoader.m_nummodels))
                            {
                                const BSPModel& model = bspLoader.m_dmodels[modelnr];
                                for (int n=0;n<model.numBrushes;n++)
                                {
                                    btAlignedObjectArray<btVector3> planeEquations;
                                    bool	isValidBrush = false;

                                    //convert brush
                                    const BSPBrush& brush = bspLoader.m_dbrushes[model.firstBrush+n];
                                    {
                                        for (int p=0;p<brush.numSides;p++)
                                        {
                                            int sideid = brush.firstSide+p;
                                            BSPBrushSide& brushside = bspLoader.m_dbrushsides[sideid];
                                            int planeid = brushside.planeNum;
                                            BSPPlane& plane = bspLoader.m_dplanes[planeid];
                                            btVector3 planeEq;
                                            planeEq.setValue(
                                                plane.normal[0],
                                                plane.normal[1],
                                                plane.normal[2]);
                                            planeEq[3] = scaling*-plane.dist;
                                            planeEquations.push_back(planeEq);
                                            isValidBrush=true;
                                        }
                                        if (isValidBrush)
                                        {
                                            
                                            btAlignedObjectArray<btVector3>	vertices;
                                            btGeometryUtil::getVerticesFromPlaneEquations(planeEquations,vertices);

                                            bool isEntity=true;
                                            addConvexVerticesCollider(vertices,isEntity,targetLocation);
                                            
                                        }
                                    }

                                }
                            }
                        } 
                        else
                        {
                            printf("unsupported trigger_push model, md3 ?\n");
                        }
                    }
                
                }
            }
        }
}
					
// USE_ENTITIES
// ...

}



#endif 
#endif //BSP_CONVERTER_H

