DOCKER_COMPOSE = docker-compose --file inaccel.yml

build:
	DOCKER_BUILDKIT=true $(DOCKER_COMPOSE) build

push:
	$(DOCKER_COMPOSE) push
