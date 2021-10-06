## flight-delay

-   xilinx_u200_xdma_201830_2
-   xilinx_u50_gen3x16_xdma_201920_3

```sh
# Download 'Flight Delays and Cancellations' data
wget --content-disposition "https://share.xelera.io/download.php?id=23&token=UJg5f1zRqTomh9nwMtg20ONWu5nzHndr&download"
unzip flight_dataset.zip

# Xilinx Alveo U200
docker inaccel run -e PWD -e access_key=$PWD/cred.json -t inaccel/apps:xelera flight-delay-u200 --

# Xilinx Alveo U50
docker inaccel run -e PWD -e access_key=$PWD/cred.json -t inaccel/apps:xelera flight-delay-u50 --
```

| Environment Variable    | Default       |
| ----------------------- | ------------- |
| `data_fpath`            | `flights.csv` |
| `enable_binomial`       | `false`       |
| `enable_FPGA_inference` | `true`        |
| `enable_multinomial`    | `true`        |
| `enable_regression`     | `false`       |
| `enable_SW_inference`   | `true`        |
| `max_depth`             | `8`           |
| `n_loops`               | `1000`        |
| `num_test_samples`      | `1000`        |
| `number_of_trees`       | `100`         |
