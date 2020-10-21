FROM fedora:32 AS builder

LABEL maintainer="Ed Beroset <beroset@ieee.org>"

# Only the first line is needed to build the software.  All packages starting
# with doxygen are only needed to generate the documentation.
RUN dnf update -y \
        && dnf install -y gcc gcc-c++ make boost-static git cmake unzip npm \
        doxygen graphviz texlive-latex texlive-linegoal texlive-multirow \
        texlive-hanging texlive-adjustbox texlive-stackengine texlive-listofitems \
        texlive-ulem texlive-wasysym texlive-collection-fontsrecommended \
        texlive-sectsty texlive-fancyhdr texlive-natbib texlive-tocloft \
        texlive-newunicodechar texlive-etoc

WORKDIR /tmp/
COPY . .
RUN mkdir ./build && \
    cd ./build && \
    cmake -Wno-dev .. && \
    make && \
    make install && \
    make pdf

WORKDIR /tmp/build/doc/html/
RUN git clone https://github.com/Freeboard/freeboard.git dashboard
COPY dashboard dashboard
RUN cd dashboard && npm install; npm install grunt-cli underscore
