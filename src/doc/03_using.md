# How to use this software # {#using}

Before the software is run, it will need to be built, which is described in [How to build this software](@ref building) and in [Native Linux build](@ref native).

This software is intended to be run in containers using `docker-compose` and so the host system must have that software installed and configured and meet the other configuration prerequisites as documented in [How to build this software](@ref building).

## Docker images ##
[Docker](https://docs.docker.com/) is software that provides a way to run applications securely isolated in a container, packaged with all its dependencies and libraries.  There is also container orchestration software called `docker-compose` which is an optional part of Docker.  Using `docker-compose` we can very simply get everything running simply by using the command:

    docker-compose up 

The process to do this is described in [How to build this software](@ref building).  For this description, it is assumed that the reader has followed the instructions there and that `docker-compose` is successfully running. 

At this point we have six docker containers:
 
 1. simulated meter
 2. simulated HES 
 3. simulated Access Point
 4. dashboard server
 5. documentation server

### The components
The interaction among the software pieces is entirely automated and periodic.  Each of the components is described below.

Each *simulated meter* first attempts to register with the HES.  The registration mechanism is to establish a TCPv6 session with the HES and send a single letter "R" (for Register).  If this is successful, the meter passively listens for DLMS/COSEM requests which can include both data reads and writes and service disconnect/reconnect commands.  See [How it works](@ref design) for details on exactly what the simulator supports.

The *simulated HES* listens for registration messages as described above.  As each registration is received, it adds the registering meter's IPv6 address to the list of registered meters.  Every 1.5 seconds, the HES sends a service connect request, followed by a read of an arbitrary data item to each of the registered meters.  It does so depending on the configuration of the Access Point.  If the Access Point is configured as *route_only*, it is operating in Mode 1 and so the HES communicates directly with the meters using DLMS/COSEM, with all IPv6 traffic routing through the Access Point.  If the Access Point is **not** configured as *route_only*, the HES uses an alternative mechanism in Mode 2.  It sends a request to the Access Point containing the desired reading size (one of *small*, *medium* or *large*) and a list of meters to be interrogated.  In the simulation, a small request asks for a data object that is 40 bytes long, a medium request is about 600 and a large request is about 20K.  

The *simulated Access Point* listens for requests from the HES as described above.  An example request might look like this:

    small,2001:3200:3200::2,2001:3200:3200::5,2001:3200:3200::6

This is a request to the Access Point to read the small data object from each of the three meters at the given IPv6 addresses.

The response might look like this:

    {"meterdata":[
    {"meter":"2001:3200:3200::2","data":"LINUXDATA0LINUXDATA0LINUXDATA0LINUXDATA0"},
    {"meter":"2001:3200:3200::5","data":"LINUXDATA0LINUXDATA0LINUXDATA0LINUXDATA0"},
    {"meter":"2001:3200:3200::6","data":"LINUXDATA0LINUXDATA0LINUXDATA0LINUXDATA0"}
    ]}

Note that in the simulation, the response is only printed to the console of the simulated Access Point machine -- no data is actually returned to the HES.  Of course this is not how a real system would work, but it is sufficient to observe the different traffic patterns depending on the operational mode of the Access Point.

The *dashboard server* is web server at `http://localhost:8081/index.html?load=dashboard.json`.  It provides a convenient means of controlling the simulation and also provides a way to see the number of packets and bytes sent and received by the Access Point's two network interfaces: both the Field Area Network connnected to the meters and the backhaul network connected to the HES.  A screen shot of the browser window is shown below:

@image html screenshot.png "Dashboard interface"
@image latex screenshot.png "Dashboard interface"

The *documentation server* is a also a separate web server serving the documentation you are now reading.  It includes not only documentation on the software and how to use it, but also detailed information on all of the files and C++ classes that make up the software components.  The documentation server maps to port 8080 on the host, so viewing the documentation is as simple as using any web browser and going to http://localhost:8080/ to see the main page of this documentation.

### Examining simulator communications with Wireshark
    As these messages are being communicating among the simulated devices, there is little visual representation of what is happening except for some logging messages that appear in the console.  However, one can use the free and open source [Wireshark](https://www.wireshark.org/#download) network packet analyzer tool to examine the traffic among the simulated machines.  

    First, let's look at the networks that `docker-compose` has created for us:

        docker network ls

This lists the networks.  The output is something like this:

    NETWORK ID          NAME                         DRIVER              SCOPE
    41c3481065f5        bridge                       bridge              local
    504ff5c36600        dlms-access-point_backhaul   bridge              local
    24b40132f3b5        dlms-access-point_docnet     bridge              local
    0d4d71970f84        dlms-access-point_meternet   bridge              local
    40b82e199c11        host                         host                local
    87a1d2962ff9        none                         null                local

The `bridge`, `host` and `none` networks are defaults created by Docker and we can ignore these for now.  The important ones are `dlms-access-point_backhaul` and `dlms-access-point_meternet`.  The `dlms-access-point_docnet` does not interact with the others and is solely used as a convenient way to separate the web server that provides this documentation.

These network ID numbers are also used by Wireshark.  If we open Wireshark and examine the available capture interfaces, we should see a list that includes devices name `br-504ff5c36600` which is the virtual bridge that is associated with the `dlms-access-point_backhaul` network and `br-0d4d71970f84` which corresponds with the `dlms-access-point_meternet` network.  If we select the `dlms-access-point_backhaul` capture interfaces and start a capture, we might see something like that shown in the picture below.

@image html Wireshark1.png "Wireshark screen shot"
@image latex Wireshark1.png "Wireshark screen shot" width=\textwidth

We can see that the HES which has address 2001:3200:3201::100:100 is communicating with two meters with addresses 2001:3200:3200::3 and 2001:3200:3200::4.  Because the server is communicating directly with the devices, we know this must mean that the Access Point is operating in Mode 1 (route only).  We can also see that frame 2238 in this capture contains the response from meter ::3 to the HES. 


### Stopping the software
There are two ways to stop the software, depending on how it was started.  If it was started using `docker-compose` without `-d`, stopping the software is simply a matter of using the `Ctrl-C` key combination to shut down docker-compose.  The software will gracefully perform the shutdown steps before returning to the command line prompt.  To also remove the virtual networks, one can run `docker-compose down` afterwards.  If the software was started with `-d`, then issuing the command `docker-compose down` shuts the software down gracefully and removes the virtual networks.  Note that it is required to issue these commands in the same directory as the `docker-compose.yml` file.

## Further reading

[Adapting the software](@ref design)
