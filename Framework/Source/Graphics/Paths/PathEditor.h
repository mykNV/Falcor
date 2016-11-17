/***************************************************************************
# Copyright (c) 2015, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************/
#pragma once
#include "Utils/Gui.h"
#include "Graphics/Paths/ObjectPath.h"
#include "Graphics/Camera/Camera.h"
#include "Utils/Gui.h"

namespace Falcor
{
    class PathEditor
    {
    public:
        using pfnEditComplete = std::function<void(void)>;
        using UniquePtr = std::unique_ptr<PathEditor>;
        using UniqueConstPtr = std::unique_ptr<const PathEditor>;

        static UniquePtr create(const ObjectPath::SharedPtr& pPath, const Camera::SharedPtr& pCamera, pfnEditComplete editCompleteCB);
        ~PathEditor();

        void render(Gui* pGui);
        void setCamera(const Camera::SharedPtr& pCamera);

    private:
        PathEditor(const ObjectPath::SharedPtr& pPath, const Camera::SharedPtr& pCamera, pfnEditComplete editCompleteCB);

        bool closeEditor(Gui* pGui);
        void editPathName(Gui* pGui);
        void editPathLoop(Gui* pGui);
        void editActiveFrameID(Gui* pGui);
        void addFrame(Gui* pGui);
        void editFrameTime(Gui* pGui);
        void editCameraProperties(Gui* pGui);

        void updateFrame(Gui* pGui);
        void deleteFrame(Gui* pGui);
        void setActiveFrameID(uint32_t id);

        ObjectPath::SharedPtr mpPath;
        Camera::SharedPtr mpCamera;
        pfnEditComplete mEditCompleteCB = nullptr;

        int32_t mActiveFrame = 0;
        float mFrameTime = 0;
    };
}