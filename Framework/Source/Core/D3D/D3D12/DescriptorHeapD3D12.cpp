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
#ifdef FALCOR_D3D12
#include "Framework.h"
#include "DescriptorHeapD3D12.h"

namespace Falcor
{
    static D3D12_DESCRIPTOR_HEAP_TYPE getHeapType(DescriptorHeap::Type type)
    {
        switch(type)
        {
        case DescriptorHeap::Type::ShaderResource:
            return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        case DescriptorHeap::Type::Sampler:
            return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        case DescriptorHeap::Type::DepthStencilView:
            return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        case DescriptorHeap::Type::RenderTargetView:
            return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        default:
            should_not_get_here();
            return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
        }
    }

    DescriptorHeap::DescriptorHeap(Type type, uint32_t descriptorsCount) : mCount(descriptorsCount), mType (type)
    {
        mDescriptorSize = getD3D12Device()->GetDescriptorHandleIncrementSize(getHeapType(type));
    }

    DescriptorHeap::~DescriptorHeap() = default;

    DescriptorHeap::SharedPtr DescriptorHeap::create(Type type, uint32_t descriptorsCount, bool shaderVisible)
    {
        DescriptorHeap::SharedPtr pHeap = SharedPtr(new DescriptorHeap(type, descriptorsCount));
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};

        desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.Type = getHeapType(type);
        desc.NumDescriptors = descriptorsCount;
        if(FAILED(getD3D12Device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pHeap->mpHeap))))
        {
            logError("Can't create descriptor heap");
            return nullptr;
        }

        return pHeap;
    }

    DescriptorHeap::CpuHandle DescriptorHeap::getHandle(uint32_t index) const
    {
        assert(index < mCurDesc);
        CpuHandle handle = mpHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += mDescriptorSize * index;
        return handle;
    }

    DescriptorHeap::CpuHandle DescriptorHeap::getFreeCpuHandle()
    {
        if(mCurDesc >= mCount)
        {
            logError("Can't find free CPU handle in descriptor heap");
            return CpuHandle();
        }

        mCurDesc++;
        return getHandle(mCurDesc - 1);
    }
}

#endif //#ifdef FALCOR_D3D12