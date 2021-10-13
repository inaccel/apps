## 2d-reverse-time-migration

- xilinx_u280_xdma_201920_1

```sh
docker inaccel -e PWD -p inaccel/apps -t xilinx run 2d-reverse-time-migration
```

| Environment Variable | Default                                |
| -------------------- | -------------------------------------- |
| `config`             | `/opt/xilinx/rtm2d/config_sample.json` |

## anti-money-laundering

- xilinx_u200_xdma_201830_2
- xilinx_u250_xdma_201830_2
- xilinx_u50_gen3x16_xdma_201920_3

```sh
docker inaccel -e PWD -p inaccel/apps -t xilinx run anti-money-laundering
```

| Environment Variable | Default                  |
| -------------------- | ------------------------ |
| `in_dir`             | `/home/nimbix/demo_data` |
| `mode`               | `0`                      |

## database-aggregation

- xilinx_u280_xdma_201920_1

```sh
docker inaccel -e PWD -p inaccel/apps -t xilinx run database-aggregation
```

| Environment Variable | Default                                        |
| -------------------- | ---------------------------------------------- |
| `in_dir`             | `/opt/xilinx/apps/vt_database/aggr/share/data` |
| `scale`              | `10`                                           |

## database-join

- xilinx_u280_xdma_201920_1

```sh
docker inaccel -e PWD -p inaccel/apps -t xilinx run database-join
```

| Environment Variable | Default                                        |
| -------------------- | ---------------------------------------------- |
| `in_dir`             | `/opt/xilinx/apps/vt_database/join/share/data` |
| `out_dir`            |                                                |
| `scale`              | `10`                                           |

## gzip-compression-decompression

- xilinx_u50_gen3x16_xdma_201920_3

```sh
docker inaccel -e PWD -p inaccel/apps -t xilinx run gzip-compression-decompression
```

| Environment Variable | Default                             |
| -------------------- | ----------------------------------- |
| `file`               | `/Xilinx_notice_and_disclaimer.txt` |
| `mode`               | `test`                              |

## integer-sorting

- xilinx_u200_xdma_201830_2

```sh
docker inaccel -e PWD -p inaccel/apps -t xilinx run integer-sorting
```

| Environment Variable | Default |
| -------------------- | ------- |
| `asc`                | `1`     |
| `files_in`           |         |
| `files_out`          |         |
| `in`                 |         |
| `out`                |         |
