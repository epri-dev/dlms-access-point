FROM nginx:alpine AS dashboard
COPY --from=beroset/dlms-access-point-builder /tmp/build/doc/html/dashboard /usr/share/nginx/html
COPY src/docker/default.conf /etc/nginx/conf.d/default.conf
