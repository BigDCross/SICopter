from math import radians, sin, cos, sqrt, asin, degrees

import GPS.GPS_Utils

class GPSCoord:
    def __init__(self, lat, lon):
        self.lat = lat
        self.lon = lon

    def __add__(self, other):
        return GPS_Utils.calc_gps_offset(self.lat, self.lon, other[0], other[1])

    def __iadd__(self, other):
        self.lat, self.lon = GPS_Utils.calc_gps_offset(self.lat, self.lon, other[0], other[1])
        return self.lat, self.lon

    def __str__(self):
        return str("<{0}, {1}>".format(self.lat, self.lon))

    def __repr__(self):
        return str("<{0}, {1}>".format(self.lat, self.lon))

class MissionGenerator:
    def __init__(self):
        pass

    def setStartingPos(self, lat, lon):
        self.starting_point = GPSCoord(lat, lon)

    def generate(self):
        pass

    def setMissionFile(self):
        pass

    def _generate_point_by_offset(self):
        pass

class MissionParser:
    def __init__(self, mission_name, output_name):
        self.mission_name = mission_name
        self.mission_file = open(mission_name, "r")
        self.mission_outfile = open(output_name, "w")

    def parse(self):
        print "Parsing!"
        lines = self.mission_file.read()
        self.mission_outfile.write(lines)
        print lines

parser = MissionParser("Missions/Fly_Square_SI_Mission.txt", "Missions/Banana.txt")

parser.parse()
