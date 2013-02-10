#include "triangle.h"
#include "scene.h"
#include "simd/simd.h"

using namespace std;

static float min(float f1, float f2, float f3) {
        return std::min(f1, std::min(f2, f3));
}

static float max(float f1, float f2, float f3) {
        return std::max(f1, std::max(f2, f3));
}

//Triangle clipper stolen from TinyRT
void clip(const scene &scene, int tid
          , const aabb& voxel, aabb& leftv, aabb &rightv, float splitf, int axis) 
{
        const auto &tri = scene.getTriangle(tid);
        vec3f verts[3]  = { scene.getVertex(tri.p0),
                            scene.getVertex(tri.p1),
                            scene.getVertex(tri.p2) };

        //kdtree should not produce a split plane that is outside the bounds of 
        //   the voxel
        if (voxel.lower[axis] > splitf || voxel.upper[axis] < splitf)
                abort();
        
        const int uaxis = modulo3[axis+1];
        const int vaxis = modulo3[axis+2];

        ssef tvert(verts[0][axis], verts[1][axis], verts[2][axis], BIG_FLOAT);
        ssef mask(tvert < splitf);
        int imask = _mm_movemask_ps(mask);
        
        int numRight = 0;
        int lidx0, lidx1, ridx0, ridx1;
        switch (imask) {
        case 1: lidx0 = 0; lidx1 = 0; ridx0 = 1; ridx1 = 2; numRight = 2; break;
        case 2: lidx0 = 1; lidx1 = 1; ridx0 = 0; ridx1 = 2; numRight = 2; break;
        case 3: lidx0 = 0; lidx1 = 1; ridx0 = 2; ridx1 = 2; numRight = 1; break;
        case 4: lidx0 = 2; lidx1 = 2; ridx0 = 0; ridx1 = 1; numRight = 2; break;
        case 5: lidx0 = 0; lidx1 = 2; ridx0 = 1; ridx1 = 1; numRight = 1; break;
        case 6: lidx0 = 1; lidx1 = 2; ridx0 = 0; ridx1 = 0; numRight = 1; break;
        default: abort();
        }
                
        ssed tleft(tvert[lidx0], tvert[lidx1]);   //vertices on left
        ssed tright(tvert[ridx0], tvert[ridx1]);  //vertices on right
        ssed tsplit(splitf, splitf);              //coordinates of split in axis

        ssed t  = (tsplit - tleft) / (tright - tleft);

        ssed uleft  = ssed(verts[lidx0][uaxis], verts[lidx1][uaxis]);
        ssed uright = ssed(verts[ridx0][uaxis], verts[ridx1][uaxis]);
        ssed usplit = uleft + (uright - uleft) * t; //coordinates of split on u axis
        usplit      = clamp(usplit
                            , ssed(voxel.lower[uaxis], voxel.lower[uaxis])
                            , ssed(voxel.upper[uaxis], voxel.upper[uaxis]));

        ssed vleft  = ssed(verts[lidx0][vaxis], verts[lidx1][vaxis]);
        ssed vright = ssed(verts[ridx0][vaxis], verts[ridx1][vaxis]);
        ssed vsplit = vleft + (vright - vleft) * t; //coordinates of split on v axis
        vsplit      = clamp(vsplit
                            , ssed(voxel.lower[vaxis], voxel.lower[vaxis])
                            , ssed(voxel.upper[vaxis], voxel.upper[vaxis]));
        
        vec3f cutmin, cutmax;
        cutmin[axis]  = splitf;
        cutmax[axis]  = splitf;
        cutmin[uaxis] = min(usplit[0], usplit[1]);
        cutmax[uaxis] = max(usplit[0], usplit[1]);
        cutmin[vaxis] = min(vsplit[0], vsplit[1]);
        cutmax[vaxis] = max(vsplit[0], vsplit[1]);

        if (numRight == 1) {
                for (int k = 0; k < 3; k++ ) {
                        // left side is AABB of two verts and two cut points
                        leftv.lower[k] = max(voxel.lower[k],
                                             min(cutmin[k], verts[lidx0][k], verts[lidx1][k]));
                        leftv.upper[k] = min(voxel.upper[k],
                                             max(cutmax[k], verts[lidx0][k], verts[lidx1][k]));

                        // right side is AABB of one vert and two cut points 
                        rightv.lower[k] = max(voxel.lower[k], min(cutmin[k], verts[ridx0][k]));
                        rightv.upper[k] = min(voxel.upper[k], max(cutmax[k], verts[ridx0][k]));
                }
        } else {
                for( int k = 0; k < 3; k++ ){
                        // right side is AABB of two verts and two cut points
                        rightv.lower[k] = max(voxel.lower[k],
                                              min(cutmin[k], verts[ridx0][k], verts[ridx1][k]));
                        rightv.upper[k] = min(voxel.upper[k],
                                              max(cutmax[k], verts[ridx0][k], verts[ridx1][k]));

                        // left side is AABB of one vert and two cut points
                        leftv.lower[k] = max(voxel.lower[k], min(cutmin[k], verts[lidx0][k]));
                        leftv.upper[k] = min(voxel.upper[k], max(cutmax[k], verts[lidx0][k]));
                }
        }

}
