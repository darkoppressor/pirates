#!/usr/bin/env python3

import sys
import logging
import os
import argparse
import shutil
import re
import requests
import io
import zipfile
import numpy
import png

logging.basicConfig(level = logging.WARNING)

# I define top left and bottom right corners using SRTM coords
# Example: N32,W101 N05,W057

argparser = argparse.ArgumentParser(description = 'Generate world from SRTM data.')
argparser.add_argument('topLeft',
                        help = 'the string representing the top left map tile')
argparser.add_argument('bottomRight',
                        help = 'the string representing the bottom right map tile')

SRTM_MAX_X_INTEGER = 180
SRTM_MIN_X_INTEGER = 0
SRTM_REQUIRED_X_DIGITS = 3
SRTM_MAX_Y_INTEGER = 59
SRTM_MIN_Y_INTEGER = -1
SRTM_REQUIRED_Y_DIGITS = 2
SRTM_SAMPLES = 1201

def getMapTileValue (valueString):
    return [valueString[0], valueString[1:]]

def incrementMapTileValue (mapTileValue, firstLetter, secondLetter, maxInteger, minInteger, requiredDigits):
    mapTileInteger = int(mapTileValue[1])

    if mapTileValue[0] == firstLetter:
        mapTileInteger += 1

        if mapTileInteger == maxInteger:
            mapTileValue[0] = secondLetter
    else:
        mapTileInteger -= 1

        if mapTileInteger == minInteger:
            mapTileValue[0] = firstLetter

            if mapTileInteger < 0:
                mapTileInteger = -mapTileInteger

    mapTileValue[1] = str(mapTileInteger).zfill(requiredDigits)

def mapTileProcessingError (mapTileX, mapTileY, chunkCoords, emptyChunks, error = False):
    prefix = 'chunk is empty: '

    if error:
        prefix = 'chunk error: '

    print(prefix + str(chunkCoords[0]) + ', ' + str(chunkCoords[1]) + ' (' + mapTileY[0] + mapTileY[1] + mapTileX[0] + mapTileX[1] + ')')

    emptyChunks.add((chunkCoords[0], chunkCoords[1]))

def processMapTile (mapTileX, mapTileY, chunkCoords, emptyChunks):
    regions = ['North_America', 'South_America', 'Islands', 'Eurasia', 'Australia', 'Africa']

    validResponse = None

    for region in regions:
        url = 'https://dds.cr.usgs.gov/srtm/version2_1/SRTM3/' + region + '/' + mapTileY[0] + mapTileY[1] + mapTileX[0] + mapTileX[1] + '.hgt.zip'

        response = requests.get(url, stream = True)

        if response.status_code == 200:
            validResponse = response
            break

    if validResponse:
        print('chunk exists: ' + str(chunkCoords[0]) + ', ' + str(chunkCoords[1]) + ' (' + mapTileY[0] + mapTileY[1] + mapTileX[0] + mapTileX[1] + ')')

        try:
            zf = zipfile.ZipFile(io.BytesIO(response.content))

            if len(zf.infolist()) == 1:
                data = zf.read(zf.infolist()[0])
            else:
                mapTileProcessingError(mapTileX, mapTileY, chunkCoords, emptyChunks, error = True)
        except:
            mapTileProcessingError(mapTileX, mapTileY, chunkCoords, emptyChunks, error = True)
        else:
            if data:
                elevations = numpy.frombuffer(data, numpy.dtype('>i2'), SRTM_SAMPLES * SRTM_SAMPLES).reshape((SRTM_SAMPLES, SRTM_SAMPLES))

                pixels = []

                for x in range(elevations.shape[0]):
                    pixels.append([])

                    for y in range(elevations.shape[1]):
                        value = int((int(elevations[x, y]) + 32768) / 256)

                        pixels[x].append(value)
                        pixels[x].append(value)
                        pixels[x].append(value)
                        pixels[x].append(255)

                f = open('data/' + str(chunkCoords[0]) + '_' + str(chunkCoords[1]) + '.png', 'wb')
                w = png.Writer(SRTM_SAMPLES, SRTM_SAMPLES, greyscale = False, alpha = True, bitdepth = 8)
                w.write(f, pixels)
                f.close()
    else:
        mapTileProcessingError(mapTileX, mapTileY, chunkCoords, emptyChunks)

def main (argv):
    args = argparser.parse_args()

    topLeft = args.topLeft
    bottomRight = args.bottomRight

    try:
        # Ensure that the inputs are valid, erroring out if they are not

        MAP_TILE_REGEX = '^[NS][0-9]+,[EW][0-9]+$'

        if not re.fullmatch(MAP_TILE_REGEX, topLeft):
            argparser.error('Invalid top left map tile')
            return

        if not re.fullmatch(MAP_TILE_REGEX, bottomRight):
            argparser.error('Invalid bottom right map tile')
            return

        # Start with an empty data directory for output

        if os.path.isdir('data'):
            shutil.rmtree('data', ignore_errors = True)

        if not os.path.isdir('data'):
            os.mkdir('data')

        # A list of chunk coordinates for chunks that have no associated SRTM data
        emptyChunks = set()

        # Split the inputs into x and y values
        topLeftComponents = topLeft.split(',')
        bottomRightComponents = bottomRight.split(',')

        # Split the x and y values into letter/digit pairs (i.e., 'N32' becomes ['N', '32'])
        topLeftX = getMapTileValue(topLeftComponents[1])
        topLeftY = getMapTileValue(topLeftComponents[0])
        bottomRightX = getMapTileValue(bottomRightComponents[1])
        bottomRightY = getMapTileValue(bottomRightComponents[0])

        # Increment the bottom right map tile coordinates, so they can act as exclusive limits
        incrementMapTileValue(bottomRightX, 'E', 'W', SRTM_MAX_X_INTEGER, SRTM_MIN_X_INTEGER, SRTM_REQUIRED_X_DIGITS)
        incrementMapTileValue(bottomRightY, 'S', 'N', SRTM_MAX_Y_INTEGER, SRTM_MIN_Y_INTEGER, SRTM_REQUIRED_Y_DIGITS)

        mapTileX = list(topLeftX)
        mapTileY = list(topLeftY)

        chunkCoords = [0, 0]

        while True:
            while True:
                processMapTile(mapTileX, mapTileY, chunkCoords, emptyChunks)

                chunkCoords[1] += 1
                incrementMapTileValue(mapTileY, 'S', 'N', SRTM_MAX_Y_INTEGER, SRTM_MIN_Y_INTEGER, SRTM_REQUIRED_Y_DIGITS)

                if mapTileY == bottomRightY:
                    break

            chunkCoords[1] = 0
            mapTileY = list(topLeftY)

            chunkCoords[0] += 1
            incrementMapTileValue(mapTileX, 'E', 'W', SRTM_MAX_X_INTEGER, SRTM_MIN_X_INTEGER, SRTM_REQUIRED_X_DIGITS)

            if mapTileX == bottomRightX:
                break

        # Write empty chunk coordinates to file
        f = open('data/emptyChunkCoords', 'w')
        for chunkCoords in emptyChunks:
            f.write(str(chunkCoords[0]) + ',' + str(chunkCoords[1]) + '\n')
        f.close()

    except Exception as e:
        logging.exception(e)

if __name__ == '__main__':
    main(sys.argv)
