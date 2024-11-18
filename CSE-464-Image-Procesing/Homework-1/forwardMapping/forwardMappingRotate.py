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

    for y in range(orgHeight):
        for x in range(orgWidth):
            xPointOrg = int(x - orgWidth / 2)
            yPointOrg = int(y - orgHeight / 2)

            xPoint = int(xPointOrg * math.cos(-angleRad) - yPointOrg * math.sin(-angleRad)) + centerLocX
            yPoint = int(xPointOrg * math.sin(-angleRad) + yPointOrg * math.cos(-angleRad)) + centerLocY

            if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                pixel_value = image.getpixel((x, y))
                newImageData[yPoint][xPoint] = list(pixel_value)

    for y in range(newHeight):
        for x in range(newWidth):
            returnedImage.putpixel((x, y), tuple(newImageData[y][x]))

    return returnedImage

rotateDegress = 60
processedImage = rotateImage(rotateDegress)
processedImage.save("istanbulRotated.jpg")
processedImage.show()