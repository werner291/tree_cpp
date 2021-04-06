
#include <execution>
#include <simLib.h>
#include <sstream>
#include <stack>
#include "tree_ext.hpp"
#include <3Vector.h>
#include <3X3Matrix.h>
#include <random>

#include "proctree.hpp"

static LIBRARY simLib;

const int PURE_SHAPE_SPHERE = 1;
const int PURE_SHAPE_CYLINDER = 2;

void make_tree(C3Vector vec3, int depth, int parent) {

    std::random_device rnd;
    std::uniform_real_distribution<float> dist(-0.5,0.5);

    float diameter = powf(sqrtf(1.0/3.0), (float) depth);
    float size[3] = { diameter, diameter, 1.0 };
    float brown[3] = {0.5,0.25,0.125};

    C3X3Matrix rot;
    rot.setEulerAngles(dist(rnd), dist(rnd), dist(rnd) );

    C3Vector offset = C3Vector(0,0,0.5);

    int cylinder = simCreatePureShape(PURE_SHAPE_CYLINDER, 8 /* respondable */, size, 1000.0, nullptr);

    simSetObjectParent(cylinder,parent,false);

    simSetObjectPosition(cylinder, parent, (vec3 + rot * offset).data);

    simSetObjectOrientation(cylinder, parent, rot.getEulerAngles().data);

    simSetShapeColor(cylinder, nullptr, sim_colorcomponent_ambient_diffuse, brown);

    if (depth < 3) {
        for (int i = 0; i < 3; i++) {
            make_tree(offset, depth + 1, cylinder);
        }
    }


}

unsigned char simStart(void *reservedPointer, int reservedInt) {

    //region Load simLib
    char curDirAndFile[1024];
#ifdef _WIN32
    #ifdef QT_COMPIL
        _getcwd(curDirAndFile, sizeof(curDirAndFile));
    #else
        GetModuleFileName(NULL,curDirAndFile,1023);
        PathRemoveFileSpec(curDirAndFile);
    #endif
#elif defined (__linux) || defined (__APPLE__)
    getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif

    std::string currentDirAndPath(curDirAndFile);
    std::string temp(currentDirAndPath);

#ifdef _WIN32
    temp+="\\coppeliaSim.dll";
#elif defined (__linux)
    temp+="/libcoppeliaSim.so";
#elif defined (__APPLE__)
    temp+="/libcoppeliaSim.dylib";
#endif /* __linux || __APPLE__ */

    simLib=loadSimLibrary(temp.c_str());
    if (simLib==NULL)
    {
        printf("TreeGen: error: could not find or correctly load coppeliaSim.dll. Cannot start the plugin.\n"); // cannot use simAddLog here.
        return(0); // Means error, CoppeliaSim will unload this plugin
    }
    if (getSimProcAddresses(simLib)==0)
    {
        printf("TreeGen: error: could not find all required functions in coppeliaSim.dll. Cannot start the plugin.\n"); // cannot use simAddLog here.
        unloadSimLibrary(simLib);
        return(0); // Means error, CoppeliaSim will unload this plugin
    }
    //endregion

    simAddLog("TreeGen", sim_verbosity_default, "Ceci n'est pas un arbre.");

    float size[3] = {1.0,1.0,1.0};

    make_tree(C3Vector::zeroVector, 0, -1);

    return 1;
}


void simEnd() {

}

struct BubbleRob {
    int motorHandles[2];
};

//static std::vector<BubbleRob> allBubbleRobs;

static int banner_id = -1;

void *simMessage(int message, int *auxiliaryData, void *customData, int *replyData) {
    
    switch (message) {
        case sim_message_scene_loaded: {



        } break;

        default:
            // ignore
            break;
    }
    
    return nullptr;
}

