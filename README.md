# Mesh network

## Build and Flash

```
python -m west build -p -b dual_inventive_stm32f407 blinky
python flash_cig.py st-link build/zephyr/zephyr.bin
```