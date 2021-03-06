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

#include "LinuxBaseLibrary.h"
#include "LinuxCOSEMServer.h"

#include "HDLCLLC.h"
#include "COSEM.h"
#include "serialwrapper/SerialWrapper.h"
#include "tcpwrapper/TCPWrapper.h"
#include "dlms-access-pointConfig.h"

#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <asio.hpp>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>

class ServerApp
{
public:
    ServerApp(EPRI::LinuxBaseLibrary& BL) :
        m_Base(BL)
    {
        m_Base.get_io_service().post(std::bind(&ServerApp::Server_Handler, this));
    }

    virtual void Run()
    {
        m_Base.Process();
    }

    virtual bool Register(const char *HESaddress) {
        bool result{false};
        try {
            static const std::string RegRequest{"R"};
            asio::ip::tcp::socket s(m_Base.get_io_service());
            asio::ip::tcp::resolver::query q(HESaddress, "4059");
            asio::ip::tcp::resolver resolver(m_Base.get_io_service());
            asio::connect(s, resolver.resolve(q));
            asio::write(s, asio::buffer(RegRequest.data(), RegRequest.size()));
            result = true;
        } catch (std::exception& err)
        {
            std::cerr << err.what() << std::endl;
        }
        return result;
    }


protected:
    void Server_Handler()
    {
        EPRI::ISocket* pSocket{EPRI::Base()->GetCore()->GetIP()->CreateSocket(
            EPRI::LinuxIP::Options(EPRI::LinuxIP::Options::MODE_SERVER, EPRI::LinuxIP::Options::VERSION6)
        )};

        std::cout << "Meter Listening on Port 4059\n";
        m_pServerEngine = new EPRI::LinuxCOSEMServerEngine(EPRI::COSEMServerEngine::Options(),
            new EPRI::TCPWrapper(pSocket));
        if (EPRI::SUCCESSFUL != pSocket->Open())
        {
            std::cout << "Failed to initiate listen\n";
            exit(0);
        }
    }

    EPRI::LinuxCOSEMServerEngine * m_pServerEngine = nullptr;
    EPRI::LinuxBaseLibrary&           m_Base;
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: Metersim HESaddress\n";
        return 1;
    }
    std::cout << "EPRI DLMS/COSEM meter simulator\n";
    bool reg = false;
    while (1) {
        EPRI::LinuxBaseLibrary     bl;
        ServerApp App(bl);
        // register with head end system
        if (reg) {
            App.Run();
        } else {
            reg = App.Register(argv[1]);
            std::cout << "Registered with " << argv[1] << "\n";
        }
    }
}
