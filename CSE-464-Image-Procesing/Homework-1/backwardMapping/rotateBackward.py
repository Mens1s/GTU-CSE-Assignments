import math
from PIL import Image

def bilinearInterpolate(image, x, y):

    x0, y0 = int(math.floor(x)), int(math.floor(y))
    x1, y1 = x0 + 1, y0 + 1

    if x0 < 0 or y0 < 0 or x1 >= image.width or y1 >= image.height:
        return (0, 0, 0)

    q11 = image.getpixel((x0, y0))  
    q21 = image.getpixel((x1, y0))  
    q12 = image.getpixel((x0, y1))  
    q22 = image.getpixel((x1, y1))  

    f_x1 = x - x0
    f_x0 = 1 - f_x1
    f_y1 = y - y0
    f_y0 = 1 - f_y1

    r = int(f_x0 * (f_y0 * q11[0] + f_y1 * q12[0]) + f_x1 * (f_y0 * q21[0] + f_y1 * q22[0]))
    g = int(f_x0 * (f_y0 * q11[1] + f_y1 * q12[1]) + f_x1 * (f_y0 * q21[1] + f_y1 * q22[1]))
    b = int(f_x0 * (f_y0 * q11[2] + f_y1 * q12[2]) + f_x1 * (f_y0 * q21[2] + f_y1 * q22[2]))

    r = max(0, min(255, r))
    g = max(0, min(255, g))
    b = max(0, min(255, b))

    return (r, g, b)

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

            pixel_value = bilinearInterpolate(image, xOldImagePoint, yOldImagePoint)
            newImageData[y][x] = list(pixel_value)

    for y in range(newHeight):
        for x in range(newWidth):
            returnedImage.putpixel((x, y), tuple(newImageData[y][x]))

    return returnedImage


rotateDegress = 60
processedImage = rotateImage(rotateDegress)
processedImage.save("istanbulRotated.jpg")
processedImage.show()