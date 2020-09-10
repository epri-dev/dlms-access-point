# How to build this software {#building} #

As mentioned in the description of [How to use this software](@ref using), there are two basic ways in which the software can be run and built.  The easy method is to build and run the software in containers using Docker.  The alternative method is to build and run the software in multiple separate windows on a computer equipped with the Linux operating system.  See [Native Linux build](@ref native).  While other alternatives are possible (e.g. running Docker on a Windows 10 machine), these instructions are described in terms of running a *version 32 (or later) of a Fedora distribution of Linux*.

## Building with Docker ##
Docker provides a lightweight alternative to a full virtual machine.  One can create an *image* file which contains the software and all of the required infrastructure to be able to run it.  These are the steps required to be able to create and run the Docker image.  

### Get Docker installed and running ###
The following steps assume that you already have Docker running on a Linux machine of some kind.  See the [Docker "getting started"](https://docs.docker.com/get-started/) pages for how to do that.  It also assumes that you have `docker-compose` available, configured and running.  The same pages will be helpful with that as well.

### Build the software ###
The simplest way to build the software within a `Docker` container is to start with the `build.sh` file. In fact, if you wish to run but not modify the software, it suffices to download only two files: the `build.sh` file and the `docker-compose.yml` file.  With only those two files, one can build the software on a Linux machine by first running `build.sh` which builds two container images, and then running `docker-compose up` to run them.

To run the `build.sh` file, you need a good connection to the internet.  The `build.sh` file executes two separate `docker` commands which will download a base Docker image and install all required tools (to the image, and not to your host computer).  This may take many minutes depending on the speed of your internet connection and computer.  Eventually the build process will be complete and two images will be created named `beroset/dlms-access-point` and `beroset/dlms-access-point-docs`.  

At this point the images are created on your hard drive and an internet connection is not longer necessary.  Running `docker-compose up` at this point will cause Docker to run four containers.  The last few lines of that should look something like this:

    Creating docker_dlms-access-point-hes_1    ... done
    Creating docker_dlms-access-point-meter1_1 ... done
    Creating docker_dlms-access-point-docs_1   ... done
    Creating docker_dlms-access-point-meter2_1 ... done
        
## Further reading

[How to use the software](@ref using)
