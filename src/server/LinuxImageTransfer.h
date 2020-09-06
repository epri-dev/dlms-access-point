// ===========================================================================
// Copyright (c) 2020, Electric Power Research Institute (EPRI)
// All rights reserved.
//
// dlms-access-point ("this software") is licensed under BSD 3-Clause license.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// *  Neither the name of EPRI nor the names of its contributors may
//    be used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// This EPRI software incorporates work covered by the following copyright and permission
// notices. You may not use these works except in compliance with their respective
// licenses, which are provided below.
//
// These works are provided by the copyright holders and contributors "as is" and any express or
// implied warranties, including, but not limited to, the implied warranties of merchantability
// and fitness for a particular purpose are disclaimed.
//
// This software relies on the following libraries and licenses:
//
// ###########################################################################
// Boost Software License, Version 1.0
// ###########################################################################
//
// * asio v1.10.8 (https://sourceforge.net/projects/asio/files/)
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "COSEM.h"
#include "COSEMDevice.h"
#include "interfaces/IData.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace EPRI
{
    const ClassIDType CLSID_ImageTransfer = 18;

    /**
     * implements the Image Transfer object (class 18, version 0)
     */
    class ImageTransfer : public ICOSEMInterface
    {
        COSEM_DEFINE_SCHEMA(DoubleLongUnsignedSchema)
        COSEM_DEFINE_SCHEMA(Image_Transfer_Status_Schema)
        COSEM_DEFINE_SCHEMA(Image_Transferred_Block_Status_Schema)
        COSEM_DEFINE_SCHEMA(Image_To_Activate_Info_Schema)
        COSEM_DEFINE_SCHEMA(Image_Transfer_Initiate_Schema)
        COSEM_DEFINE_SCHEMA(Image_Block_Transfer_Schema)

    public :
        ImageTransfer();
        virtual ~ImageTransfer() = default;

        enum Attributes : ObjectAttributeIdType
        {
            ATTR_IMAGE_BLOCK_SIZE = 2,
            ATTR_IMAGE_TRANSFERRED_BLOCKS_STATUS,
            ATTR_IMAGE_FIRST_NOT_TRANSFERRED_BLOCK_NUMBER,
            ATTR_IMAGE_TRANSFER_ENABLED,
            ATTR_IMAGE_TRANSFER_STATUS,
            ATTR_IMAGE_TO_ACTIVATE_INFO,
        };

        enum Image_Transfer_Status_Schema : uint8_t
        {
            IMAGE_TRANSFER_NOT_INITIATED = 0,
            IMAGE_TRANSFER_INITIATED,
            IMAGE_VERIFICATION_INITIATED,
            IMAGE_VERIFICATION_SUCCESSFUL,
            IMAGE_VERIFICATION_FAILED,
            IMAGE_ACTIVATION_INITIATED,
            IMAGE_ACTIVATION_SUCCESSFUL,
            IMAGE_ACTIVATION_FAILED,
        };

        COSEMAttribute<ATTR_IMAGE_BLOCK_SIZE, DoubleLongUnsignedSchema, 0x08> image_block_size;
        COSEMAttribute<ATTR_IMAGE_TRANSFERRED_BLOCKS_STATUS, Image_Transferred_Block_Status_Schema, 0x18> image_transferred_blocks_status;
        COSEMAttribute<ATTR_IMAGE_FIRST_NOT_TRANSFERRED_BLOCK_NUMBER, DoubleLongUnsignedSchema, 0x20> image_first_not_transferred_block_number;
        COSEMAttribute<ATTR_IMAGE_TRANSFER_ENABLED, BooleanSchema, 0x28> image_transfer_enabled;
        COSEMAttribute<ATTR_IMAGE_TRANSFER_STATUS, Image_Transfer_Status_Schema, 0x30> image_transfer_status;
        COSEMAttribute<ATTR_IMAGE_TO_ACTIVATE_INFO, Image_To_Activate_Info_Schema, 0x38> image_to_activate_info;

        enum Methods : ObjectAttributeIdType
        {
            METHOD_IMAGE_TRANSFER_INITIATE = 1,
            METHOD_IMAGE_BLOCK_TRANSFER,
            METHOD_IMAGE_VERIFY,
            METHOD_IMAGE_ACTIVATE,
        };
        COSEMMethod<METHOD_IMAGE_TRANSFER_INITIATE, Image_Transfer_Initiate_Schema, 0x40> image_transfer_initiate;
        COSEMMethod<METHOD_IMAGE_BLOCK_TRANSFER, Image_Block_Transfer_Schema, 0x48> image_block_transfer;
        COSEMMethod<METHOD_IMAGE_VERIFY, IntegerSchema, 0x40> image_verify;
        COSEMMethod<METHOD_IMAGE_ACTIVATE, IntegerSchema, 0x58> image_activate;
    };


    class IImageTransfer : public ImageTransfer, public ICOSEMObject
    {
    public:
        IImageTransfer() = delete;
        IImageTransfer(const COSEMObjectInstanceCriteria& OIDCriteria,
                uint16_t ShortNameBase = std::numeric_limits<uint16_t>::max());
        virtual ~IImageTransfer() = default;
    protected:
        virtual APDUConstants::Action_Result InternalAction(const AssociationContext& Context,
            ICOSEMMethod * pMethod,
            const Cosem_Method_Descriptor& Descriptor,
            const DLMSOptional<DLMSVector>& Parameters,
            DLMSVector * pReturnValue = nullptr) = 0;
    };

    class LinuxImageTransfer : public IImageTransfer
    {
    public:
        LinuxImageTransfer();

    protected:
        virtual APDUConstants::Data_Access_Result InternalGet(const AssociationContext& Context,
            ICOSEMAttribute * pAttribute,
            const Cosem_Attribute_Descriptor& Descriptor,
            SelectiveAccess * pSelectiveAccess) final override;
        virtual APDUConstants::Action_Result InternalAction(const AssociationContext& Context,
            ICOSEMMethod * pMethod,
            const Cosem_Method_Descriptor& Descriptor,
            const DLMSOptional<DLMSVector>& Parameters,
            DLMSVector * pReturnValue = nullptr) final;

    private:
        /// The size of a block in bytes
        static constexpr std::size_t block_size{512};
        /// The number of blocks in an image
        static constexpr std::size_t block_count{256};
        using ImageBlock = std::array<std::uint8_t, block_size>;
        std::array<ImageBlock, block_count> image;
        std::uint8_t status{IMAGE_TRANSFER_NOT_INITIATED};
        DLMSBitSet block_status;
        DLMSVector image_id;
        std::size_t image_size;

        /// the block number of the first block not yet transferred
        unsigned first_not_transferred_block_number{0};
    };
}
