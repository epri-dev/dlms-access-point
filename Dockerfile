FROM fedora:32 AS builder

LABEL maintainer="Ed Beroset <beroset@ieee.org>"

# Only the first line is needed to build the software.  All packages starting
# with doxygen are only needed to generate the documentation.
RUN dnf update -y \
        && dnf install -y gcc gcc-c++ make git cmake unzip npm \
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
    make pdf

WORKDIR /tmp/build/doc/html/
RUN git clone https://github.com/Freeboard/freeboard.git dashboard
RUN cp /tmp/dashboard/package.json dashboard/package.json
RUN cd dashboard && npm install; npm install grunt-cli underscore

FROM nginx:alpine AS dashboard
COPY --from=builder /tmp/build/doc/html/dashboard /usr/share/nginx/html
COPY src/docker/default.conf /etc/nginx/conf.d/default.conf

FROM nginx:alpine AS docserver
COPY --from=builder /tmp/build/doc/html/ /usr/share/nginx/html
COPY --from=builder /tmp/build/doc/latex/refman.pdf /usr/share/nginx/html/dlms-access-point.pdf
COPY src/docker/default.conf /etc/nginx/conf.d/default.conf

FROM fedora:32 AS demo
RUN dnf update -y \
    && dnf install -y iputils iproute nmap-ncat
WORKDIR /tmp/
COPY --from=builder /tmp/build/src/DLMS_sim .
COPY --from=builder /tmp/build/src/HESsim .
COPY --from=builder /tmp/build/src/Metersim .
