FROM nginx:alpine AS dashboard
COPY --from=beroset/dlms-access-point-builder /tmp/build/doc/html/dashboard /usr/share/nginx/html
COPY --from=beroset/dlms-access-point-builder /tmp/src/docker/docserver/default.conf /etc/nginx/conf.d/default.conf
