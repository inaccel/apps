## HEflow Dev Test

#### gRPC Server

```sh
docker inaccel -e PWD -p inaccel/heflow-dev run project .
docker inaccel -p inaccel/heflow-dev run model models:/he_sigmoid/1
```

#### gRPC Client

```sh
docker inaccel -e PWD -p inaccel/heflow-dev run shell python he_sigmoid.py
```
