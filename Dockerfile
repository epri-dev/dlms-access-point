FROM fedora:32 AS builder
ARG projectsourcename

LABEL maintainer="Ed Beroset <beroset@ieee.org>"

# Only the first line is needed to build the software.  All packages starting
# with doxygen are only needed to generate the documentation.
RUN dnf update -y \
        && dnf install -y gcc gcc-c++ make git cmake unzip \
        doxygen graphviz texlive-latex texlive-linegoal texlive-multirow \
        texlive-hanging texlive-adjustbox texlive-stackengine texlive-listofitems \
        texlive-ulem texlive-wasysym texlive-collection-fontsrecommended \
        texlive-sectsty texlive-fancyhdr texlive-natbib texlive-tocloft \
        texlive-newunicodechar texlive-etoc

WORKDIR /tmp/
RUN mkdir ./build && \
    cd ./build && \
    cmake -Wno-dev .. && \
    make && \
    make pdf

FROM nginx:alpine AS docserver
COPY --from=builder /tmp/dlms-access-point/build/doc/html/ /usr/share/nginx/html
COPY --from=builder /tmp/dlms-access-point/build/doc/latex/refman.pdf /usr/share/nginx/html/dlms-access-point.pdf
COPY src/docker/default.conf /etc/nginx/conf.d/default.conf

FROM fedora:32 AS demo
RUN dnf update -y
WORKDIR /tmp/
COPY --from=builder /tmp/dlms-access-point/build/src/DLMS_sim .
COPY --from=builder /tmp/dlms-access-point/build/src/HESsim .
COPY --from=builder /tmp/dlms-access-point/build/src/Metersim .
