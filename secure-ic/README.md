## trng

-   xilinx_aws-vu9p-f1_dynamic-shell
-   xilinx_u200_xdma_201830_2

```sh
# AWS VU9P
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t secure-ic run trng-aws --

# Xilinx Alveo U200
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t secure-ic run trng-u200 --
```

| Environment Variable | Default |
| -------------------- | ------- |
| `mb_qty`             | `128`   |
