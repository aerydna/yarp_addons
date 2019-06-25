/*
 * Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_MATH_TRANSFORM_H
#define YARP_MATH_TRANSFORM_H

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include "api.h"
#include <yarp/math/Math.h>
#include <yarp/math/Quaternion.h>

namespace yarp
{
namespace math
{

class YARP_ADDONS_API FrameTransform
{
public:

    std::string parentFrame;
    std::string frameId;
    YARP_DEPRECATED_MSG("use parentFrame and frameId instead") std::string src_frame_id;
    YARP_DEPRECATED_MSG("use parentFrame and frameId instead") std::string dst_frame_id;
    double      timestamp{ 0 };

    struct Translation_t
    {
        double tX;
        double tY;
        double tZ;

        void set(double x, double y, double z)
        {
            tX = x;
            tY = y;
            tZ = z;
        }
    } translation;

    Quaternion rotation;

    FrameTransform()
    {
        timestamp = 0;
        translation.set(0, 0, 0);
    }

    FrameTransform(const std::string& parent, const std::string& child, yarp::sig::Matrix m, bool staticTf = false) : parentFrame(parent), frameId(child)
    {
        fromMatrix(m);
        if (!staticTf)
        {
            timestamp = yarp::os::Time::now();
        }
    }

    FrameTransform
        (
        const std::string& parent,
        const std::string& child,
        double             inTX,
        double             inTY,
        double             inTZ,
        double             inRX,
        double             inRY,
        double             inRZ,
        double             inRW
        ) : parentFrame(parent),
            frameId(child)
    {
        src_frame_id = parent;
        dst_frame_id = child;
        translation.set(inTX, inTY, inTZ);
        rotation.w() = inRW;
        rotation.x() = inRX;
        rotation.y() = inRY;
        rotation.z() = inRZ;
    }

    ~FrameTransform(){};

    void transFromVec(double X, double Y, double Z)
    {
        translation.set(X, Y, Z);
    }

    void rotFromRPY(double R, double P, double Y)
    {
        double               rot[3] = { R, P, Y };
        size_t               i = 3;
        yarp::sig::Vector    rotV;
        yarp::sig::Matrix    rotM;
        rotV = yarp::sig::Vector(i, rot);
        rotM = rpy2dcm(rotV);
        rotation.fromRotationMatrix(rotM);
    }

    yarp::sig::Vector getRPYRot() const
    {
        yarp::sig::Vector rotV;
        yarp::sig::Matrix rotM;
        rotM = rotation.toRotationMatrix4x4();
        rotV = dcm2rpy(rotM);
        return rotV;
    }

    yarp::sig::Matrix toMatrix() const
    {
        yarp::sig::Vector rotV;
        yarp::sig::Matrix t_mat(4,4);
        t_mat = rotation.toRotationMatrix4x4();
        t_mat[0][3] = translation.tX;
        t_mat[1][3] = translation.tY;
        t_mat[2][3] = translation.tZ;
        return t_mat;
    }

    bool fromMatrix(const yarp::sig::Matrix& mat)
    {
        if (mat.cols() != 4 || mat.rows() != 4)
        {
            yError("FrameTransform::fromMatrix() failed, matrix should be = 4x4");
            yAssert(mat.cols() == 4 && mat.rows() == 4);
            return false;
        }

        yarp::sig::Vector q;

        translation.tX = mat[0][3];
        translation.tY = mat[1][3];
        translation.tZ = mat[2][3];
        rotation.fromRotationMatrix(mat);
        return true;
    }

    std::string toString() const
    {
        char buff[1024];
        sprintf(buff, "%s -> %s \n tran: %f %f %f \n rot: %f %f %f %f \n\n",
            src_frame_id.c_str(),
            dst_frame_id.c_str(),
            translation.tX,
            translation.tY,
            translation.tZ,
            rotation.x(),
            rotation.y(),
            rotation.z(),
            rotation.w());
        return std::string(buff);
}
};

}
}
#endif

