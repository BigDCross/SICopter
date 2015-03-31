from math import radians, sin, cos, sqrt, asin, degrees

from GPS.GPS_Utils import haversine, calc_gps_offset

import sys

class GPSCoord:
    def __init__(self, lon, lat):
        self.lon = float(lon)
        self.lat = float(lat)

    def __add__(self, other):
        new_lon, new_lat = calc_gps_offset(self.lon, self.lat, other[0], other[1])
        return GPSCoord(new_lon, new_lat)

    def __iadd__(self, other):
        self.lon, self.lat = calc_gps_offset(self.lon, self.lat, other[0], other[1])
        return GPSCoord(self.lon, self.lat)

    def __str__(self):
        return str("<{0}, {1}>".format(self.lon, self.lat))

    def __repr__(self):
        return str("<{0}, {1}>".format(self.lon, self.lat))

class MissionGenerator:
    def __init__(self):
        pass

    def setStartingPos(self, lon, lat):
        self.starting_point = GPSCoord(lon, lat)

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
        lines = self.mission_file.readlines()

        for i, curr_line in enumerate(lines):
            tokens = curr_line.split("\t")

            # First line contains file format info
            if i == 0:
                self.mission_outfile.write("QGC WPL 110\n")

            # Write out second line unchanged
            elif i == 1:
                curr_line = curr_line[0:-2] # Kill newline at end (this might fail on windows)
                self.mission_outfile.write(curr_line)
                self.mission_outfile.write("\n")

                self.starting_point = GPSCoord(tokens[9], tokens[8]) # tokens[8]: lat tokens[9]: lon
                next_pos = self.starting_point
            else:
                # dy is north(+)/south(-)
                # dx is east(+)/west(-)
                dy = float(tokens[8])
                dx = float(tokens[9])
                next_pos += (dx, dy)

                for j, tok in enumerate(tokens):
                    # Write all tokens unchanged except for 8 and 9 (lat and lon)
                    # and 11 which just needs a newline removed
                    if j == 8:
                        self.mission_outfile.write(str(next_pos.lat))
                        self.mission_outfile.write("\t")
                    elif j == 9:
                        self.mission_outfile.write(str(next_pos.lon))
                        self.mission_outfile.write("\t")
                    elif j == 11:
                        tok = tok[0:-2] # Kill newline at end
                        self.mission_outfile.write(tok)
                        self.mission_outfile.write("\n")
                    else:
                        self.mission_outfile.write(tok)
                        self.mission_outfile.write("\t")

        self.mission_file.close()
        self.mission_outfile.close()

def main():
    if len(sys.argv) != 3:
        print "Usage: python Mission_Generator.py <SI_Mission> <Output_Mission_Name>"
        sys.exit()
    else:
        parser = MissionParser(sys.argv[1], sys.argv[2])
        parser.parse()

main()
