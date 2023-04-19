# examples

1. `inspect_geotiff` (Show info about a GeoTIFF, roughly mimicking `gdalinfo`
2. `map_extent` (Display the GeoTIFF's extent on a Leaflet map)
3. `thumbnail` (Generate false-color PNG thumbnail of first three bands of a GeoTIFF)
4. `thumbnail_map` (Generate false-color PNG thumbnail, warp to EPSG:3857, display on Leaflet map)
5. `tile_tiff` (Generate false-color PNG map tiles, warp to EPSG:3857, display tile by tile on Leaflet map)

## run

For example, in windows:

```sh
python.exe examples/emrun.py --browser chrome --port 8027 examples\qt_opengl_gui\hellogles3.htm
```
