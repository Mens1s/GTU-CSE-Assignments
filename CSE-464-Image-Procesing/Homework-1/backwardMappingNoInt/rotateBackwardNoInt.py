import math
from PIL import Image

def rotateImage(rotateDegrees=60):
    angleRad = rotateDegrees * math.pi / 180

    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size

    newWidth = int(abs(math.cos(angleRad) * orgWidth) + abs(math.sin(angleRad) * orgHeight))
    newHeight = int(abs(math.sin(angleRad) * orgWidth) + abs(math.cos(angleRad) * orgHeight))

    returnedImage = Image.new("RGB", (newWidth, newHeight))

    centerLocX = int(newWidth / 2)
    centerLocY = int(newHeight / 2)

    newImageData = [[[0 for _ in range(3)] for _ in range(newWidth)] for _ in range(newHeight)]

    for y in range(newHeight):
        for x in range(newWidth):
            xPointOrg = x - centerLocX
            yPointOrg = y - centerLocY

            xOldImagePoint = int(xPointOrg * math.cos(angleRad) - yPointOrg * math.sin(angleRad)) + int(orgWidth / 2)
            yOldImagePoint = int(xPointOrg * math.sin(angleRad) + yPointOrg * math.cos(angleRad)) + int(orgHeight / 2)

            if 0 <= xOldImagePoint < orgWidth and 0 <= yOldImagePoint < orgHeight:
                pixel_value = image.getpixel((xOldImagePoint, yOldImagePoint))
                newImageData[y][x] = list(pixel_value)

    for y in range(newHeight):
        for x in range(newWidth):
            returnedImage.putpixel((x, y), tuple(newImageData[y][x]))

    return returnedImage

rotateDegress = 60
processedImage = rotateImage(rotateDegress)
processedImage.save("istanbulRotated.jpg")
processedImage.show()