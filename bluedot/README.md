## deepfield-sr

- xilinx_aws-vu9p-f1_dynamic-shell

```sh
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t bluedot run deepfield-sr --
```

| Environment Variable | Default                                |
| -------------------- | -------------------------------------- |
| `access_key`         |                                        |
| `bitrate`            | `20M`                                  |
| `codec`              | `libx264`                              |
| `duration`           | `5`                                    |
| `filter_graph`       | `scale_startrek=w=iw*2:h=ih*2:fpga=f1` |
| `infile`             | `/app/test_720p.mp4`                   |
| `outfile`            | `output_4k.mp4`                        |
| `time_off`           | `0`                                    |
