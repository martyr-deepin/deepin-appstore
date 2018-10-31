
all: build

build: app

app: web create-docker release-docker

web: prepare-node-image
	sudo docker run --rm -v $(CURDIR):/src -w /src node:10 \
		./node_modules/.bin/ng build --configuration=production \
		--output-path=./deploy/dist/china --base-href /china/ --deploy-url /china/
	sudo docker run --rm -v $(CURDIR):/src -w /src node:10 \
		./node_modules/.bin/ng build --configuration=international \
		--output-path=./deploy/dist/international --base-href /international/ --deploy-url /international/

prepare-node-image:
	sudo docker pull node:10

# Create docker image
create-docker: prepare-nginx-image rm-docker
	cd deploy && sudo docker build --tag dstore-appstore:latest .
	
prepare-nginx-image:
	# Make sure tag of nginx is same as in Dockerfile.
	sudo docker pull nginx:1.15

rm-docker:
	sudo docker image rm dstore-appstore || true

# Push docker image to docker hub
release-docker:
	# First delete old tag
	sudo docker image rm hub.deepin.io/xushaohua/dstore-appstore:latest || true
	sudo docker tag dstore-appstore:latest hub.deepin.io/xushaohua/dstore-appstore:latest
	sudo docker push hub.deepin.io/xushaohua/dstore-appstore:latest

.PHONY: web
