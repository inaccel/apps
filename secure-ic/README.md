## trng

- xilinx_u200_xdma_201830_2

```sh
docker inaccel -e PWD -e access_key=$PWD/cred.json -t inaccel/apps:secure-ic run trng --
```

| Environment Variable | Default |
| -------------------- | ------- |
| `mb_qty`             | `128`   |
