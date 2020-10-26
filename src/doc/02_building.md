# How to build this software {#building} #

As mentioned in the description of [How to use this software](@ref using), this software is intended to be run in software containers using `docker` and `docker-compose. While other alternatives are possible (e.g. running Docker on a Windows 10 machine), these instructions are described in terms of running a *version 32 (or later) of a Fedora distribution of Linux*.

## Building with Docker ##
Docker provides a lightweight alternative to a full virtual machine.  One can create an *image* file which contains the software and all of the required infrastructure to be able to run it.  These are the steps required to be able to create and run the Docker image.  

### Get Docker installed and running ###
The following steps assume that you already have Docker running on a Linux machine of some kind.  See the [Docker "getting started"](https://docs.docker.com/get-started/) pages for how to do that.  It also assumes that you have `docker-compose` available, configured and running.  The same pages will be helpful with that as well.  

### Configure the host machine ###
The host machine must be configured to support IPv6.  By default, most recent builds of Fedora are, but it is worth verifying if there are any problems encountered.  Also, in order to exercise all functions of the simulated network, it is required that the firewall is configured for IP Masquerade.  The [documentation](https://docs.fedoraproject.org/en-US/Fedora/19/html/Security_Guide/sec-Configure_IP_Address_Masquerading.html) for configuring IP Masquerading may be helpful for this.  

### Build the software ###
The simplest way to build the software within a `Docker` container is to start by building directly from the github repository.  That can be done with this single command:

    docker build --target builder -t beroset/dlms-access-point-builder git@github.com:epri-dev/dlms-access-point.git#main

The process can take several minutes, depending on the speed of your internet connection and computer.  Once this is build, you will now have a new docker image on your machine named `beroset/dlms-access-point-builder` which is required for building the rest of the software.

    docker build --target dashboard  -t beroset/dlms-access-point-dashboard git@github.com:epri-dev/dlms-access-point.git#main:src/docker/dashboard
    docker build --target demo  -t beroset/dlms-access-point git@github.com:epri-dev/dlms-access-point.git#main:src/docker/demo
    docker build --target docserver -t beroset/dlms-access-point-docs git@github.com:epri-dev/dlms-access-point.git#main:src/docker/docserver
    
All four of these steps are encapsulated in the `build.sh` file. In fact, if you wish to run but not modify the software, it suffices to download only two files: the `build.sh` file and the `docker-compose.yml` file.  With only those two files, one can build the software on a Linux machine by first running `build.sh` which builds two container images, and then running `docker-compose up` to run them.

To run the `build.sh` file, you need a good connection to the internet.  The `build.sh` file executes four separate `docker` commands which will download a base Docker image and install all required tools (to the image, and not to your host computer).  This may take many minutes depending on the speed of your internet connection and computer.  Eventually the build process will be complete and four images will be created named `beroset/dlms-access-point-builder`,`beroset/dlms-access-point`, `beroset/dlms-access-point-dashboard` and `beroset/dlms-access-point-docs`.  

Note that the `git` commands above use `ssh` (Secure Shell).  If you are behind a proxy or firewall that prevents outbound SSH traffic, you will have to modify these to use the HTTPS (HyperText Transport Protocol with transport layer Security) instead.  That is, everywhere that has something like this:

    git@github.com:epri-dev/dlms-access-point.git

Change it to this:

    https://github.com/epri-dev/dlms-access-point.git

At this point the images are created on your hard drive and an internet connection is not longer necessary.  Running `docker-compose up -d` at this point will cause Docker to run six containers.  The last few lines of that should look something like this:

    Creating network "dlms-access-point_meternet" with driver "bridge"
    Creating network "dlms-access-point_backhaul" with driver "bridge"
    Creating network "dlms-access-point_docnet" with the default driver
    Creating dlms-access-point_dashboard_1    ... done
    Creating dlms-access-point_hes_1          ... done
    Creating dlms-access-point_meter_1        ... done
    Creating dlms-access-point_docs_1         ... done
    Creating dlms-access-point_access-point_1 ... done

At this point the simulation is running with one meter, one access point, one HES, the documentation server and the dashboard but there is no output to the screen.  To see details as the simulation is running, omit the `-d` at the end of the `docker-compose up`.  To run with multiple simulated meters, one can use the `--scale` modifier.  For example, to run the simulation with three simulated meters, the command would be:

    docker-compose up --scale meter=3

To gracefully shut the simulation down, use `docker-compose down`.  

To learn more about what to do from here, see:

[How to use the software](@ref using)
