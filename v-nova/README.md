## ffmpeg

-   xilinx_u200_xdma_201830_2

```sh
docker inaccel -e PWD -e access_key=$PWD/cred.json -p inaccel/apps -t v-nova run ffmpeg --
```

| Environment Variable | Default                                   |
| -------------------- | ----------------------------------------- |
| `base_encoder`       | `ngcodec_sdx_hevc`                        |
| `codec`              | `pplusenc_hevc`                           |
| `eil_params`         | `accel_type=xilinx_xma;encoding_transform_type=dd;temporal_enabled=0;temporal_use_refresh=0;api_mode=asynchronous;baseEncType=ngcodec;xclbinHasNGC=1;encoding_downsample_luma=area;encoding_upsample=cubic;rate_control_mode=variableratio;rc_bucket_duration_ms=2000;encoding_step_width_model=v1;encoding_step_width_min_qp=41;bitrate_max_base_prop=0.8;bitrate_base_prop=0.8;perseus_m^Ce=robust;qp_min=30;xma_config=/vnova_pplus/SystemCfg_194.yaml` |
| `frame_size`         | `3840x2160`                               |
| `infile`              | `/samples/sample_3840x2160_8bit_P420.yuv` |
| `outfile`              | `/data/output.ts` |
| `pixel_format`       | `yuv420p`                                 |
| `stream_loop`        | `3`                                       |
| `video_bitrate`      | `8000k`                                   |
| `video_codec`        | `rawvideo`                                |
