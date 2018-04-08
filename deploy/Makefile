
all: build

build: install-deps update-submodules build-web

install-deps:
	which git || sudo apt install git -y
	sudo which docker-compose || sudo apt install docker-compose -y

update-submodules:
	git submodule update --init --recursive
	git submodule update --remote

pull-docker-image:
	sudo docker pull node

build-web: pull-docker-image
	cd .. && \
	sudo docker run --rm -v $(CURDIR):/src -w /src/src/web node \
		./node_modules/.bin/ng build -v --output-path=../deploy/dist
