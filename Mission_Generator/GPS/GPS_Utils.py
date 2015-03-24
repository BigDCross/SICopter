from math import radians, sin, cos, sqrt, asin, degrees

# Just an approximation
Earth_Radius = 6371.009

# Returns distance between two gps coords in METERS
def haversine(lat1, lon1, lat2, lon2):
    dLat = radians(lat2 - lat1)
    dLon = radians(lon2 - lon1)
    lat1 = radians(lat1)
    lat2 = radians(lat2)

    a = sin(dLat/2)**2 + cos(lat1)*cos(lat2)*sin(dLon/2)**2
    c = 2*asin(sqrt(a))

    return Earth_Radius * c * 1000

# Returns gps coords based on north/south and east/west offset in METERS
def calc_gps_offset(lat, lon, dx, dy):
    new_lat = lat + degrees(dy/(Earth_Radius * 1000.0))
    new_lon = lon + degrees(dx/(Earth_Radius * 1000.0) / cos(radians(lat)))

    return new_lat, new_lon
