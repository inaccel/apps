## trng

-   xilinx_aws-vu9p-f1_dynamic_5_0
-   xilinx_u200_xdma_201830_2

```sh
# AWS VU9P
docker inaccel -e PWD -e access_key=$PWD/cred.json -t inaccel/apps:secure-ic run trng-aws --


# Xilinx Alveo U200
docker inaccel -e PWD -e access_key=$PWD/cred.json -t inaccel/apps:secure-ic run trng-u200 --
```

| Environment Variable | Default |
| -------------------- | ------- |
| `mb_qty`             | `128`   |
