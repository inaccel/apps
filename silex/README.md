## trng

-   xilinx_u200_xdma_201830_2
-   xilinx_u250_gen3x16_xdma-3.1_202020_3
-   xilinx_u50_gen3x16_xdma_201920_3

```sh
# Xilinx Alveo U200
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t silex run trng-u200 --

# Xilinx Alveo U250
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t silex run trng-u250 --

# Xilinx Alveo U50
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t silex run trng-u50 --
```

| Environment Variable | Default |
| -------------------- | ------- |
| `num32`              | `1000`  |
