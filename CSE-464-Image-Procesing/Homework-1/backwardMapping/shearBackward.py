from PIL import Image
import math

def bilinearInterpolate(image, x, y):
    x0, y0 = int(math.floor(x)), int(math.floor(y))
    x1, y1 = min(x0 + 1, image.width - 1), min(y0 + 1, image.height - 1)

    if x0 < 0 or y0 < 0 or x0 >= image.width or y0 >= image.height:
        return (0, 0, 0)

    q11 = image.getpixel((x0, y0))
    q21 = image.getpixel((x1, y0))
    q12 = image.getpixel((x0, y1))
    q22 = image.getpixel((x1, y1))

    f_x1 = x - x0
    f_y1 = y - y0

    r = (q11[0] * (1 - f_x1) * (1 - f_y1) +
         q21[0] * f_x1 * (1 - f_y1) +
         q12[0] * (1 - f_x1) * f_y1 +
         q22[0] * f_x1 * f_y1)
    
    g = (q11[1] * (1 - f_x1) * (1 - f_y1) +
         q21[1] * f_x1 * (1 - f_y1) +
         q12[1] * (1 - f_x1) * f_y1 +
         q22[1] * f_x1 * f_y1)
    
    b = (q11[2] * (1 - f_x1) * (1 - f_y1) +
         q21[2] * f_x1 * (1 - f_y1) +
         q12[2] * (1 - f_x1) * f_y1 +
         q22[2] * f_x1 * f_y1)

    r = max(0, min(255, int(round(r))))
    g = max(0, min(255, int(round(g))))
    b = max(0, min(255, int(round(b))))

    return (r, g, b)

def shearImage(shearX=1.4, shearY=0):
    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size
 
    newWidth  = int(orgHeight * shearX) + orgWidth
    newHeight = int(orgWidth * shearY)+ orgHeight

    returnedImage = Image.new("RGB", (newWidth, newHeight))

    for y in range(newHeight):
        for x in range(newWidth):
            xPointOld = x - int(y * shearX)
            yPointOld = y - int(x * shearY)

            pixel_value = bilinearInterpolate(image, xPointOld, yPointOld)
            returnedImage.putpixel((x, y), pixel_value)

    return returnedImage

processedImage = shearImage()
processedImage.save("istanbulSheared.jpg")
processedImage.show()
