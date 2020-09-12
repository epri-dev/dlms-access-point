# DLMS/COSEM (IEC 62056) access point

Electricity meters commonly implement Device Language Message Specification and Companion Specification for Energy Metering (DLMS/COSEM) standardized in the IEC 62056 series.  Between meters in the field and a Head-End System (HES), metering systems often employ an intermediate device (sometimes called a collector or take-out point) that we are calling an access point.  This device is often attached to two different network interfaces.  One interface communicates with the meters, over a radio frequency (RF) or powerline carrier communications system.  This first network, which we refer to a a *Field Area Network* (FAN) is typically relatively low bandwidth and sometimes high latency if the FAN is built as a mesh system.  The second network, which we call here the *backhaul network* is often a higher speed, higher bandwidth and lower latency network.  Examples of such networks commonly employed include cellular modem, fiber or point-to-point radio system, but it is also possible for this link to be the same kind of communications as is used for the FAN.

This project is intended to be a reference implementation and experimental platform for an access point and associated network and devices.  It is intended to be run via software containers (such as [Docker](https://www.docker.com/) or [Podman](https://podman.io/)) and has various options controllable via a web interface that also exists in a container.

## How to use
The simplest way to use the software is to build it directly from the github repository.  Download just the `build.sh` file and `docker-compose.yml` file and execute these two commands:

    ./build.sh
    docker-compose up --scale meter=3

This will create a simulated network with three meters, an access point, HES and dashboard, and a docserver.
