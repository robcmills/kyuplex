#include "kx.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;


void kx::InitSounds ()
{
    scaleUp = sound->addSoundSourceFromFile ("./sounds/saw_C3F3G3C4_16up.ogg");
    scaleUp->setDefaultVolume (.5f);
    sound->play2D (scaleUp); // game start sound

    scaleDown = sound->addSoundSourceFromFile ("./sounds/saw_C4G3F3C3_16down.ogg");
    scaleDown->setDefaultVolume (.5f);


    // sine sounds
    sineC3 = sound->addSoundSourceFromFile ("./sounds/sineC3.ogg"); 
    sineC3s = sound->addSoundSourceFromFile ("./sounds/sineC3#.ogg"); 
    sineD3 = sound->addSoundSourceFromFile ("./sounds/sineD3.ogg"); 
    sineE3 = sound->addSoundSourceFromFile ("./sounds/sineE3.ogg"); 
    sineF3 = sound->addSoundSourceFromFile ("./sounds/sineF3.ogg"); 
    sineG3 = sound->addSoundSourceFromFile ("./sounds/sineG3.ogg"); 
    sineA3 = sound->addSoundSourceFromFile ("./sounds/sineA3.ogg"); 
    sineB3 = sound->addSoundSourceFromFile ("./sounds/sineB3.ogg"); 
    sineC4 = sound->addSoundSourceFromFile ("./sounds/sineC4.ogg"); 
    sineD4 = sound->addSoundSourceFromFile ("./sounds/sineD4.ogg"); 

    sineC3->setDefaultVolume (.5f);
    sineC3s->setDefaultVolume (.5f);
    sineD3->setDefaultVolume (.5f);
    sineE3->setDefaultVolume (.5f);
    sineF3->setDefaultVolume (.5f);
    sineG3->setDefaultVolume (.5f);
    sineA3->setDefaultVolume (.5f);
    sineB3->setDefaultVolume (.5f);
    sineC4->setDefaultVolume (.5f);
    sineD4->setDefaultVolume (.5f);

    // triangle tones
    triC1 = sound->addSoundSourceFromFile ("./sounds/triC1.ogg"); 
    triD1 = sound->addSoundSourceFromFile ("./sounds/triD1.ogg"); 
    triE1 = sound->addSoundSourceFromFile ("./sounds/triE1.ogg"); 
    triF1 = sound->addSoundSourceFromFile ("./sounds/triF1.ogg"); 
    triG1 = sound->addSoundSourceFromFile ("./sounds/triG1.ogg"); 
    triA1 = sound->addSoundSourceFromFile ("./sounds/triA1.ogg"); 
    triB1 = sound->addSoundSourceFromFile ("./sounds/triB1.ogg"); 
    triC2 = sound->addSoundSourceFromFile ("./sounds/triC2.ogg"); 
    triD2 = sound->addSoundSourceFromFile ("./sounds/triD2.ogg"); 
    triE2 = sound->addSoundSourceFromFile ("./sounds/triE2.ogg"); 
    triF2 = sound->addSoundSourceFromFile ("./sounds/triF2.ogg"); 
    triG2 = sound->addSoundSourceFromFile ("./sounds/triG2.ogg"); 
    triA2 = sound->addSoundSourceFromFile ("./sounds/triA2.ogg"); 
    triB2 = sound->addSoundSourceFromFile ("./sounds/triB2.ogg"); 
    triC3 = sound->addSoundSourceFromFile ("./sounds/triC3.ogg"); 
    triD3 = sound->addSoundSourceFromFile ("./sounds/triD3.ogg"); 
    triE3 = sound->addSoundSourceFromFile ("./sounds/triE3.ogg"); 
    triF3 = sound->addSoundSourceFromFile ("./sounds/triF3.ogg"); 
    triG3 = sound->addSoundSourceFromFile ("./sounds/triG3.ogg"); 
    triA3 = sound->addSoundSourceFromFile ("./sounds/triA3.ogg"); 
    triB3 = sound->addSoundSourceFromFile ("./sounds/triB3.ogg"); 
    triC4 = sound->addSoundSourceFromFile ("./sounds/triC4.ogg"); 

    triC1->setDefaultVolume (.5f);
    triD1->setDefaultVolume (.5f);
    triE1->setDefaultVolume (.5f);
    triF1->setDefaultVolume (.5f);
    triG1->setDefaultVolume (.5f);
    triA1->setDefaultVolume (.5f);
    triB1->setDefaultVolume (.5f);
    triC2->setDefaultVolume (.5f);
    triD2->setDefaultVolume (.5f);
    triE2->setDefaultVolume (.5f);
    triF2->setDefaultVolume (.5f);
    triG2->setDefaultVolume (.5f);
    triA2->setDefaultVolume (.5f);
    triB2->setDefaultVolume (.5f);
    triC3->setDefaultVolume (.5f);
    triD3->setDefaultVolume (.5f);
    triE3->setDefaultVolume (.5f);
    triF3->setDefaultVolume (.5f);
    triG3->setDefaultVolume (.5f);
    triA3->setDefaultVolume (.5f);
    triB3->setDefaultVolume (.5f);
    triC4->setDefaultVolume (.5f);


    triC1release = sound->addSoundSourceFromFile ("./sounds/triC1release.ogg"); 
    triC1release->setDefaultVolume (.5f);

}
