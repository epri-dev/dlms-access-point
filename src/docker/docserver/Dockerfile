FROM nginx:alpine AS docserver
COPY --from=beroset/dlms-access-point-builder /tmp/build/doc/html/ /usr/share/nginx/html
COPY --from=beroset/dlms-access-point-builder /tmp/build/doc/latex/refman.pdf /usr/share/nginx/html/dlms-access-point.pdf
COPY default.conf /etc/nginx/conf.d/default.conf

