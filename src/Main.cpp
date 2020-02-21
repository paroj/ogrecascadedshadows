/*  Copyright 2010-2012 Matthew Paul Reid
    This file is subject to the terms and conditions defined in
    file 'License.txt', which is part of this source code package.
*/
#include "ShadowCameraSetupStableCSM.h"
#include "CSMGpuConstants.h"

#include <Ogre.h> 
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>
 
using namespace Ogre; 
 

class Application : public OgreBites::ApplicationContext
{
public:
	Application() :
		mGpuConstants(0)
	{
	}

	~Application()
	{
		delete mGpuConstants;
	}

	void loadResources(void)
	{
		// Setup shadow GPU parameters before we load the resource groups so materials can use the parameters
		static const int maxCascades = 4;
		mGpuConstants = new CSMGpuConstants(maxCascades);

		OgreBites::ApplicationContext::loadResources();
	}

    void setup()
    {
        OgreBites::ApplicationContext::setup();
        mSceneMgr = getRoot()->createSceneManager();

        auto cam = mSceneMgr->createCamera("MainCam");
        cam->setAutoAspectRatio(true);
        cam->setNearClipDistance(1);
        auto camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        camNode->attachObject(cam);
        getRenderWindow()->addViewport(cam);

        auto camman = new OgreBites::CameraMan(camNode);
        camman->setStyle(OgreBites::CS_ORBIT);
        addInputListener(camman);

		setupGeometry();
		setupLights();
		setupShadows();
    }

private:
	void setupGeometry()
	{
		float scale = 0.5f;
		Vector3 scaleVec(scale, scale, scale);
		{
			SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Entity* athene = mSceneMgr->createEntity( "athene", "athene.mesh" );
			athene->setMaterialName("Examples/Athene/Basic");
			node->attachObject( athene );
			node->translate(0,-27 * scale, 0);
			node->yaw(Degree(90));
			node->setScale(scaleVec);
		}

		{
			SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			Entity* athene = mSceneMgr->createEntity( "athene2", "athene.mesh" );
			athene->setMaterialName("Examples/Athene/Basic");
			node->attachObject( athene );
			node->translate(30,-27 * scale, 22);
			node->yaw(Degree(-45));
			float scale = 0.1f;
			node->setScale(scaleVec);
		}

		Entity* pEnt;
		// Columns
		for (int x = -4; x <= 4; ++x)
		{
			for (int z = -4; z <= 4; ++z)
			{
				if (x != 0 || z != 0)
				{
					StringUtil::StrStreamType str;
					str << "col" << x << "_" << z;
					SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
					pEnt = mSceneMgr->createEntity( str.str(), "column.mesh" );
					pEnt->setMaterialName("Examples/Rockwall");
					node->attachObject( pEnt );
					node->translate(x*500,0, z*500);
					node->setScale(scale * 2, scale, scale * 2);
				}
			}

		}

		// Skybox
		mSceneMgr->setSkyBox(true, "Examples/StormySkyBox");

		// Floor plane (use POSM plane def)
		Plane* plane = new MovablePlane("*mPlane");
		plane->normal = Vector3::UNIT_Y;
		plane->d = 107 * scale;
		MeshManager::getSingleton().createPlane("MyPlane",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *plane,
			6000,6000,50,50,true,1, 25.0f / scale, 25.0f / scale, Vector3::UNIT_Z);
		Entity* pPlaneEnt = mSceneMgr->createEntity( "plane", "MyPlane" );
		pPlaneEnt->setMaterialName("Examples/Rockwall");
		pPlaneEnt->setCastShadows(false);
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);
	}

	void setupLights()
	{
		mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
		Light* light = mSceneMgr->createLight();
		light->setType(Light::LT_DIRECTIONAL);
		Vector3 direction(1,-1,0.4);
		light->setDirection(direction.normalisedCopy());
		light->setCastShadows(true);
		light->setShadowFarClipDistance(12000);
	}

	void setupShadows()
	{
		// Scene manager shadow settings
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);
		mSceneMgr->setShadowCasterRenderBackFaces(false);

		static int textureCount = 4;
		mSceneMgr->setShadowTextureCount(textureCount);
		mSceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, textureCount);

		for (int i=0; i < textureCount; i++)
			mSceneMgr->setShadowTextureConfig(i, 1024, 1024, Ogre::PF_FLOAT32_R);

		float farClip = 5000.0f;

		// Create the CSM shadow setup
		StableCSMShadowCameraSetup* shadowSetup = new StableCSMShadowCameraSetup(mGpuConstants);

		float lambda = 0.93f; // lower lamdba means more uniform, higher lambda means more logarithmic
		float firstSplitDist = 50.0f;

		shadowSetup->calculateSplitPoints(textureCount, firstSplitDist, farClip, lambda);
		StableCSMShadowCameraSetup::SplitPointList points = shadowSetup->getSplitPoints();

		// Apply settings
		shadowSetup->setSplitPoints(points);
		float splitPadding = 1.0f;
		shadowSetup->setSplitPadding(splitPadding);
		mSceneMgr->setShadowCameraSetup(ShadowCameraSetupPtr(shadowSetup));
	}

	CSMGpuConstants* mGpuConstants;
    SceneManager* mSceneMgr;
};



int main(int argc, char **argv)
{
	Application app;
    app.initApp();
	app.getRoot()->startRendering();
    app.closeApp();

    return 0; 
}
