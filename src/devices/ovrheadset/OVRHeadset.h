/*
 * Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef YARP_OVRHEADSET_OVRHEADSET_H
#define YARP_OVRHEADSET_OVRHEADSET_H

#include "ImageType.h"

#include <yarp/os/PeriodicThread.h>
#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/ServiceInterfaces.h>
#include <yarp/dev/IJoypadController.h>

#include <yarp/dev/IFrameSource.h>
#include <mutex>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/Image.h>

#include <GL/glew.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <map>
#include <vector>


/**
* @ingroup dev_impl_other
*
* \section SDLJoypad Description of input parameters
* \brief Device that reads inputs of Joypads compatible with the SDL library.
*
* Parameters accepted in the config argument of the open method:
* |   Parameter name      | Type   | Units | Default Value | Required  | Description                               | Notes |
* |:---------------------:|:------:|:-----:|:-------------:|:---------:|:-----------------------------------------:|:-----:|
* | tfLocal               | string |       |               | yes       | local port name receiving and posting tf  |       |
* | tfRemote              | string |       |               | yes       | name of the transformServer port          |       |
* | tf_left_hand_frame    | string |       |               | Yes       | name of the left hand frame               |       |
* | tf_right_hand_frame   | string |       |               | yes       | name of the right hand frame              |       |
* | tf_root_frame         | string |       |               | yes       | name of the root frame                    |       |
* | stick_as_axis         | bool   |       |               | yes       | if axes shoud be published as sticks      |       |
* | gui_elements          | int    |       |               | yes       | number of the gui element to visualize    |       |

Gui Groups parameters
* |   Parameter name      | Type   | Units | Default Value | Required  | Description               | Notes |
* | width                 | double | pixel |               | yes       | width of the widget       |       |
* | height                | double | pixel |               | yes       | height of the widget      |       |
* | x                     | double | pixel |               | yes       | x position of the widget  |       |
* | y                     | double | pixel |               | yes       | y position of the widget  |       |
* | z                     | double | pixel |               | yes       | z position of the widget  |       |
* | alpha                 | double |       |               | yes       | alpha value of the widget |       |
**/


namespace yarp { namespace os { template <typename T> class BufferedPort; }}
namespace yarp { namespace os { class Bottle; }}
struct GLFWwindow;
class InputCallback;
class TextureStatic;
class TextureBattery;
struct guiParam;

namespace yarp {
namespace dev {

class OVRHeadset : public yarp::dev::DeviceDriver,
                   public yarp::os::PeriodicThread,
                   public yarp::dev::IService,
                   public yarp::dev::IJoypadController
                  ,public yarp::dev::ImplementIFrameSource
{
protected:
    void updateFrameContainer(FrameEditor& frameContainer) override;
    bool callbackPrepare() override;
    bool callbackDismiss() override;

public:
    explicit OVRHeadset();
    virtual ~OVRHeadset();

    // yarp::dev::DeviceDriver methods
    bool open(yarp::os::Searchable& cfg) override;
    bool close() override;

    // yarp::os::RateThread methods
    bool threadInit() override;
    void threadRelease() override;
    void run() override;

    //  yarp::dev::IService methods
    bool startService() override;
    bool updateService() override;
    bool stopService() override;

    // yarp::dev::IJoypadController methods
    bool getAxisCount(unsigned int& axis_count) override;
    bool getButtonCount(unsigned int& button_count) override;
    bool getTrackballCount(unsigned int& Trackball_count) override;
    bool getHatCount(unsigned int& Hat_count) override;
    bool getTouchSurfaceCount(unsigned int& touch_count) override;
    bool getStickCount(unsigned int& stick_count) override;
    bool getStickDoF(unsigned int stick_id, unsigned int& DoF) override;
    bool getButton(unsigned int button_id, float& value) override;
    bool getTrackball(unsigned int trackball_id, yarp::sig::Vector& value) override;
    bool getHat(unsigned int hat_id, unsigned char& value) override;
    bool getAxis(unsigned int axis_id, double& value) override;
    bool getStick(unsigned int stick_id, yarp::sig::Vector& value, JoypadCtrl_coordinateMode coordinate_mode) override;
    bool getTouch(unsigned int touch_id, yarp::sig::Vector& value) override;

private:

    typedef yarp::os::BufferedPort<yarp::sig::FlexImage> FlexImagePort;

    bool createWindow(int w, int h);
    void onKey(int key, int scancode, int action, int mods);
    void reconfigureRendering();
    void reconfigureFOV();

    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwErrorCallback(int error, const char* description);
    static void ovrDebugCallback(uintptr_t userData, int level, const char* message);
    static void DebugHmd(ovrHmdDesc hmdDesc);
    void errorManager(ovrResult error);
    void fillAxisStorage();
    void fillErrorStorage();
    void fillButtonStorage();
    void fillHatStorage();
    void resetInput();

    std::vector<guiParam> huds;
    InputCallback* displayPorts[2]{ nullptr, nullptr };
    ovrEyeRenderDesc EyeRenderDesc[2];
    TextureStatic* textureLogo{ nullptr };
    ovrLayerQuad logoLayer;
    TextureStatic* textureCrosshairs{ nullptr };
    ovrLayerQuad crosshairsLayer;
    TextureBattery* textureBattery{ nullptr };
    ovrLayerQuad batteryLayer;
    ovrMirrorTexture mirrorTexture{ nullptr };
    GLuint mirrorFBO{ 0 };
    ovrSession session;
    ovrHmdDesc hmdDesc;
    GLFWwindow* window{ nullptr };
    ovrTrackingState ts;
    ovrPoseStatef headpose;
    ovrPoseStatef predicted_headpose;
    unsigned int guiCount;
    bool         enableGui{ true };
    bool threadIdValid;
    std::mutex frameMutex;
    std::vector<std::pair<std::string*, ovrPoseStatef*>> poses
    {
        {&rightFrame, &ts.HandPoses[ovrHand_Right]},
        {&leftFrame, &ts.HandPoses[ovrHand_Left]  },
        {&headFrame, &headpose },
        {&headFramePredicted, &predicted_headpose }
    };

    yarp::os::Mutex                  inputStateMutex;
    ovrInputState                    inputState;
    bool                             inputStateError{ false };
    bool                             getStickAsAxis;
    std::vector<ovrButton>           buttonIdToOvrButton;
    std::vector<float*>              axisIdToValue;
    std::map<int, int>               DButtonToHat;
    std::map<ovrResult, std::string> error_messages;

    std::string leftFrame;
    std::string rightFrame;
    std::string rootFrame;
    std::string headFrame;
    std::string headFramePredicted;

    bool closed{ false };
    long long distortionFrameIndex{ 0 };

    unsigned int texWidth;
    unsigned int texHeight;
    double camHFOV[2];
    size_t camWidth[2];
    size_t camHeight[2];
    ovrFovPort fov[2];

    bool flipInputEnabled{ false };
    bool imagePoseEnabled{ true };
    bool userPoseEnabled{ false };

    // Layers
    bool logoEnabled{ true };
    bool crosshairsEnabled{ true };
    bool batteryEnabled{ true };

    double prediction;

}; // class OVRHeadset

} // namespace dev
} // namespace yarp


#endif // YARP_OVRHEADSET_OVRHEADSET_H
