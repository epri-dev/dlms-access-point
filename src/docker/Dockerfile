FROM fedora:32 AS builder
ARG projectsourcename

LABEL maintainer="Ed Beroset <beroset@ieee.org>"

# Only the first line is needed to build the software.  All packages starting
# with doxygen are only needed to generate the documentation.
RUN dnf update -y \
        && dnf install -y gcc gcc-c++ make git cmake unzip boost-static \
        doxygen graphviz texlive-latex texlive-linegoal texlive-multirow \
        texlive-hanging texlive-adjustbox texlive-stackengine texlive-listofitems \
        texlive-ulem texlive-wasysym texlive-collection-fontsrecommended \
        texlive-sectsty texlive-fancyhdr texlive-natbib texlive-tocloft \
        texlive-newunicodechar texlive-etoc

WORKDIR /tmp/
# When the software repository is no longer private, this will work 
# without requiring authentication:
# RUN git clone --recurse-submodules https://github.com/epri-dev/dlms-access-point.git 
ADD $projectsourcename.tar.gz .
RUN mv $projectsourcename dlms-access-point && \
    mkdir dlms-access-point/build && \
    cd /tmp/dlms-access-point/build && \
    cmake -Wno-dev .. && \
    make && \
    make pdf

WORKDIR /tmp/dlms-access-point/build/doc/html/
RUN git clone https://github.com/Freeboard/freeboard.git dashboard
RUN cp /tmp/dlms-access-point/dashboard/package.json dashboard/package.json
RUN cd dashboard && npm install; npm install grunt-cli underscore

FROM nginx:alpine AS docserver
COPY --from=builder /tmp/dlms-access-point/build/doc/html/ /usr/share/nginx/html
COPY --from=builder /tmp/dlms-access-point/build/doc/latex/refman.pdf /usr/share/nginx/html/dlms-access-point.pdf
COPY default.conf /etc/nginx/conf.d/default.conf

FROM fedora:32 AS demo
RUN dnf update -y
WORKDIR /tmp/
COPY --from=builder /tmp/dlms-access-point/build/src/DLMS_sim .
COPY --from=builder /tmp/dlms-access-point/build/src/HESsim .
COPY --from=builder /tmp/dlms-access-point/build/src/Metersim .
