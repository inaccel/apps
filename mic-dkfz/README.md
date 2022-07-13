## mic-dkfz

```sh
# fpga tracking
docker inaccel -e PWD -p inaccel/apps -t mic-dkfz run Tracking -- <args>

# cpu tracking (using cython)
docker inaccel -e PWD -e tag=cythonize -p inaccel/apps -t mic-dkfz run Tracking -- <args>

#tractseg
docker inaccel -e PWD -p inaccel/apps -t mic-dkfz run TractSeg -- <args>
```
