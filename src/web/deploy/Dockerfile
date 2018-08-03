
FROM nginx:1.15

LABEL maintainer="XuShaohua<xushaohua@deepin.com>"
LABEL description="AppStore Review Server for Dstore"

COPY dist /usr/share/nginx/html
COPY default.conf /etc/nginx/conf.d/

# Make resource readonly
RUN chmod -R 555 /usr/share/nginx/html

#EXPOSE 8000
#USER www-data:www-data

#CMD ["nginx", "-g", "daemon off;"]
