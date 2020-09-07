# Introduction {#mainpage} #

Electricity metering systems commonly implement Device Language Message Specification and Companion Specification for Energy Metering (DLMS/COSEM) standardized in the IEC 62056 series. Both the meter and the Head-End Systems (HES) communicate using this application layer protocol over some underlying network or communications channel.  It is often the case that there is an intermediate device located between the meters and the HES that is called an *access point*.

Such access points are often on two different kinds of networks.  The Field Area Network (FAN) is the one on which the meters participate and may be a low-speed wireless mesh network or a powerline carrier system.  The other network, often called the *backhaul* network is often a higher speed, higher bandwidth and lower latency network, over media such as fiber or cellular phone, although it could also be the same technology as is used for the FAN.

There are three fundamental ways in which such an access point operates:

 1. as a lower layer router only
 2. as a store-and-forward network
 3. as a hybrid device performing both functions

# Purpose 
This tool is both a demonstration of all three kinds of access point, but also serves as an experimental platform.  Using this code, interested persons can quickly and easily set up a small network, modify paramters and then observe the results.

# Access point modes
As mentioned earlier, there are three fundamental ways in which such an access point operates:

 1. as a lower layer router only
 2. as a store-and-forward network
 3. as a hybrid device performing both functions

For convenience, we will refer to these in the rest of the document as *Mode 1*, *Mode 2* and *Mode 3*.

## Mode 1
In a Mode 1 Access Point, the DLMS protocol need not be present or used.  It is, effectively "part of the network" from the standpoint of application layer protocols.  This mode has the advantage of simplicity since it eliminates the need for the Access Point to have a DLMS protocol stack, but also eliminates the need for it to securely store security tokens (keys or passwords) for application layer protocols such as DLMS.  For the purposes of this simulation, such a device adds a small bit of latency to the network but is otherwise invisible from the application layer.  

This mode has some potential drawbacks.  For instance, if the HES needs to read all of the meters connected to an Access Point, it will have to reach each device individually and the sum of all communications latencies will be the minimum time required to do so.  Routing information and performance data that the node collects will also have to either be shared with the system via some other application layer protocol (possibly requiring some other security tokens and mechanisms) or be inaccessible to operators of the system.

## Mode 2
Mode 2 Access Points include a DLMS protocol implementation and are thus participating nodes in their network.  There are different ways to implement such a device. One way is to implement a standard protocol on the FAN to communicate with meter on behalf of the HES, and implement some other protcol on the backhaul side.  For instance, the HES might use a proprietary protocol to ask the Access Point for daily meter readings of a list of devices.  To improve the latency of the aggregate task, the Access Point might have been programmed to automatically poll all meters *before* the HES asks for the data, thus effectively caching the responses in anticipation of a regular request from the HES such as a daily read.  

With that said, however, it's probable that a system operator will want the ability to interrogate some particular meter or meters on an ad-hoc basis, as to check for current status or to operate a service disconnect or reconnect.  Effectively, this means that a Mode 2 Access Point will have to either provide or simulate the function of a Mode 1 device.  If it provides the service via an application layer protocol or "wrapper", it's still a Mode 2 device.  However if it provides true routing at a lower layer, it becomes a Mode 3 device.

## Mode 3
A Mode 3 Access Point implements a hybrid of the Mode 1 and Mode 2 functions.  That is, such a device includes a DLMS communications protocol implementation, but also provides lower layer routing which is, as described in Mode 1, a lower layer function which does not need or use DLMS or any other application layer protocol.  Such devices provide all of the benefits of both Mode 1 and Mode 2 devices, but also incur all of the costs, including operational and design complexity, added requirements of security token storage.  However, it has the potential ability to both provide a performance advantage in daily use (e.g. performing daily reads of all meters) but also provide a benefit when other things are required.

**TODO: chop the rest, but I want to remember the old docs and their structure.**

Every system that uses both DLMS/COSEM for the meters and CIM for the enterprise interface must necessarily implement a mapping from one to the other. While this has been described in [IEC TS 62056-6-9](https://webstore.iec.ch/publication/24736), this work is to show a concrete example of how this mapping might be accomplished and to document some of the ambiguities and difficulties encountered along the way.

The purpose of this tool is to provide a reference implementation of a translation from DLMS/COSEM to the Common Information Model (CIM) as would be done in an Advanced Metering Infrastructure (AMI) Head-End System (HES).  It is intended not as an actual functional HES, but as an example of the mapping from DLMS/COSEM to CIM and back, and also to serve as a way to examine the communications flows among the simulated components as though they were part of a real AMI system.

@image html blocks.svg "Block Diagram"
@image latex blocks.pdf "Block Diagram"

In its default operating mode, the software will create two simulated meters, a simulated HES and a simulated CIS.  The interaction among the software pieces is entirely automated and periodic.  Every ten seconds, the CIS sends a disconnect request, encoded as CIM, to the HES simulator.  The HES simulator, in turn, translates this into disconnect message in DLMS format and relays these commands to the two simulated meters.  After the simulated meters receive the commands, they respond to indicate that they have performed the disconnect.  The HES recieves these responses, encapsulates them into a single CIM response and finally sends this response message back to the CIS.  The CIS then delays for two seconds and sends service reconnect messages to the HES which performs the actions as before, except with a reconnect command instead of a disconnect command.  



## Further reading
[How it works](@ref design)

[How to build the software](@ref building)

[How to use the software](@ref using)
