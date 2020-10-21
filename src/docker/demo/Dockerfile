FROM fedora:32 AS demo
RUN dnf update -y \
    && dnf install -y iputils iproute nmap-ncat
WORKDIR /tmp/
COPY --from=beroset/dlms-access-point-builder /tmp/build/src/DLMS_sim .
COPY --from=beroset/dlms-access-point-builder /tmp/build/src/HESsim .
COPY --from=beroset/dlms-access-point-builder /tmp/build/src/Metersim .
COPY --from=beroset/dlms-access-point-builder /tmp/src/docker/demo/docker-entrypoint.sh .
