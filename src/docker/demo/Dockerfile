FROM fedora:32 AS demo
RUN dnf update -y \
    && dnf install -y iputils iproute nmap-ncat
WORKDIR /tmp/
COPY --from=beroset/dlms-access-point-builder /usr/local/bin /usr/local/bin
