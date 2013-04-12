
//#include "kx.h"
#include "kxPlayerCube.h"
#include "kxQuadPlane.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void kx::ProcessPlayerMovement()
{
    if( playerCube->state != EPS_DEAD )
    {
        vector3df oldPos= playerCube->getPosition();
        vector3df newPos= oldPos;

        // playerCube->moveDir = forward from camera
        playerCube->moveDir = oldPos - playerCam->getAbsolutePosition();
        playerCube->moveDir.Y = 0; // No vertical movement
        playerCube->moveDir.normalize();


        vector3df strafeVec = playerCube->moveDir;
        strafeVec = strafeVec.crossProduct (vector3df (0,1,0));

        if (playerCube->state == EPS_CUBE) 
        {
            for (u32 i=0; i< keyMapArray.size(); i++)
            {
                // MOVE_FORWARD
                if (keyMapArray[i].action==EPA_MOVE_FORWARD && keyMapArray[i].pressed)
                    newPos += playerCube->moveDir * playerCube->moveSpeed * (f32)deltaTime; 

                // MOVE_BACKWARD
                if (keyMapArray[i].action==EPA_MOVE_BACKWARD && keyMapArray[i].pressed)
                    newPos -= playerCube->moveDir * playerCube->moveSpeed * (f32)deltaTime; 

                // MOVE_LEFT
                if (keyMapArray[i].action==EPA_MOVE_LEFT && keyMapArray[i].pressed)
                    newPos += strafeVec * playerCube->moveSpeed * (f32)deltaTime; 

                // MOVE_RIGHT
                if (keyMapArray[i].action==EPA_MOVE_RIGHT && keyMapArray[i].pressed)
                    newPos -= strafeVec * playerCube->moveSpeed * (f32)deltaTime; 

                // MOVE_UP
                if (keyMapArray[i].action==EPA_MOVE_UP && keyMapArray[i].pressed)
                    newPos += vector3df (0,1,0) * playerCube->moveSpeed * (f32)deltaTime; 

                // MOVE_DOWN
                if (keyMapArray[i].action==EPA_MOVE_DOWN && keyMapArray[i].pressed)
                    newPos -= vector3df (0,1,0) * playerCube->moveSpeed * (f32)deltaTime; 

            }

            const vector3df& newVec= newPos - oldPos;

            if( controlType==ECT_IMPULSE )
            {
                if( now - playerCube->lastImpTime > 1 )  // apply 1 impulse per .001 seconds
                {
                    if( controlNodeID==KNT_PLAYER_CUBE )
                    {
                        playerCube->rb->applyCentralImpulse(( const btVector3&) newVec );
                        playerCube->lastImpTime= now;
                    }
                }
            }
            else if( controlType == ECT_NO_PHYSICS )
            {
                //if( controlNodeID == KNT_QUAD_PLANE )
                    pControlNode->setPosition( newPos );

                playerCube->setPosition( newPos );
                camPivot->setPosition( newPos );
                playerCam->setTarget( newPos );
            }


            // TODO: implement this with key event timing...
            //else if (controlType==ECT_VELOCITY)
                //playerCube->rb->setLinearVelocity ((const btVector3&) newVel);


        }
    }
}

