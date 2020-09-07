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

#include "LinuxCOSEMServer.h"
#include "COSEMAddress.h"
#include "LinuxDisconnect.h"

namespace EPRI
{
    COSEM_BEGIN_SCHEMA(Disconnect::Control_State_Schema)
        COSEM_ENUM_TYPE
        (
            {
                IDisconnect::DISCONNECTED,
                IDisconnect::CONNECTED,
                IDisconnect::READY_FOR_RECONNECTION
            }
         )
     COSEM_END_SCHEMA

     COSEM_BEGIN_SCHEMA(Disconnect::Control_Mode_Schema)
        COSEM_ENUM_TYPE
        (
            {
                IDisconnect::ATTR_CONTROL_MODE_0,
                IDisconnect::ATTR_CONTROL_MODE_1,
                IDisconnect::ATTR_CONTROL_MODE_2,
                IDisconnect::ATTR_CONTROL_MODE_3,
                IDisconnect::ATTR_CONTROL_MODE_4,
                IDisconnect::ATTR_CONTROL_MODE_5,
                IDisconnect::ATTR_CONTROL_MODE_6,
            }
        )
     COSEM_END_SCHEMA

    IDisconnect::IDisconnect(const COSEMObjectInstanceCriteria& OIDCriteria,
        uint16_t ShortNameBase /* = std::numeric_limits<uint16_t>::max() */)
        : ICOSEMObject(OIDCriteria, ShortNameBase)
    {
    }

    Disconnect::Disconnect()
        : ICOSEMInterface(CLSID_Disconnect, 0, 0, 1)
    {
        COSEM_BEGIN_ATTRIBUTES
            COSEM_ATTRIBUTE(output_state)
            COSEM_ATTRIBUTE(control_state)
            COSEM_ATTRIBUTE(control_mode)
        COSEM_END_ATTRIBUTES

        COSEM_BEGIN_METHODS
            COSEM_METHOD(remote_disconnect)
            COSEM_METHOD(remote_reconnect)
        COSEM_END_METHODS

    }


    //
    // Data
    //
    LinuxDisconnect::LinuxDisconnect()
        : IDisconnect({ 0, 0, 96, 3, 10, 255 })
    {
    }

    APDUConstants::Data_Access_Result LinuxDisconnect::InternalGet(const AssociationContext& Context,
        ICOSEMAttribute * pAttribute,
        const Cosem_Attribute_Descriptor& Descriptor,
        SelectiveAccess * pSelectiveAccess)
    {
        APDUConstants::Data_Access_Result result=APDUConstants::Data_Access_Result::object_unavailable;
        switch (pAttribute->AttributeID) {
            case ATTR_OUTPUT_STATE:
                pAttribute->Append(m_connected);
                result = APDUConstants::Data_Access_Result::success;
                break;
            case ATTR_CONTROL_STATE: 
                pAttribute->Append(m_ControlState);
                result = APDUConstants::Data_Access_Result::success;
                break;
            case ATTR_CONTROL_MODE:
                pAttribute->Append(m_ControlMode);
                result = APDUConstants::Data_Access_Result::success;
                break;
        }
        return result;
    }

    APDUConstants::Data_Access_Result LinuxDisconnect::InternalSet(const AssociationContext& Context,
        ICOSEMAttribute * pAttribute,
        const Cosem_Attribute_Descriptor& Descriptor,
        const DLMSVector& Data,
        SelectiveAccess * pSelectiveAccess)
    {
        APDUConstants::Data_Access_Result result = APDUConstants::Data_Access_Result::temporary_failure;
        switch (pAttribute->AttributeID) {
            case ATTR_OUTPUT_STATE:
            case ATTR_CONTROL_STATE:
                result = APDUConstants::Data_Access_Result::read_write_denied;
                break;
            case ATTR_CONTROL_MODE:
                {
                DLMSValue Value;
                auto RetVal = ICOSEMObject::InternalSet(Context, pAttribute, Descriptor, Data, pSelectiveAccess);
                if (APDUConstants::Data_Access_Result::success == RetVal &&
                pAttribute->GetNextValue(&Value) == COSEMType::GetNextResult::VALUE_RETRIEVED)
                {
                    // m_ControlMode = Value.unchecked_get();
                    result = APDUConstants::Data_Access_Result::success;
                }
                }
                break;
        }
        return result;
    }

    APDUConstants::Action_Result LinuxDisconnect::InternalAction(const AssociationContext& Context,
        ICOSEMMethod * pMethod,
        const Cosem_Method_Descriptor& Descriptor,
        const DLMSOptional<DLMSVector>& Parameters,
        DLMSVector * pReturnValue /*= nullptr*/)
    {
        APDUConstants::Action_Result result=APDUConstants::Action_Result::object_unavailable;
        switch (pMethod->MethodID)
        {
        case METHOD_REMOTE_DISCONNECT:
            std::cout << "Disconnect ACTION Received" << std::endl;
            if (m_ControlMode != ATTR_CONTROL_MODE_0) {
                m_connected = false;
            }
            result = APDUConstants::Action_Result::success;
            break;
        case METHOD_REMOTE_RECONNECT:
            std::cout << "Reconnect ACTION Received" << std::endl;
            switch (m_ControlMode) {
                case ATTR_CONTROL_MODE_1:
                case ATTR_CONTROL_MODE_3:
                case ATTR_CONTROL_MODE_5:
                case ATTR_CONTROL_MODE_6:
                    m_ControlState = READY_FOR_RECONNECTION;
                    result = APDUConstants::Action_Result::success;
                    break;
                case ATTR_CONTROL_MODE_2:
                case ATTR_CONTROL_MODE_4:
                    m_ControlState = CONNECTED;
                    m_connected = true;
                    result = APDUConstants::Action_Result::success;
                    break;
            }
            break;
        default:
            std::cout << "Unknown Disconnect ACTION Received" << std::endl;
            break;
        }
        //
        // TODO
        //
        return result;
    }
}
