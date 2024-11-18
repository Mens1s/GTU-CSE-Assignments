from PIL import Image

def shearImage(shearX=1.4, shearY=0):
    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size
 
    newWidth  = int(orgHeight * shearX) + orgWidth
    newHeight = int(orgWidth * shearY)+ orgHeight

    returnedImage = Image.new("RGB", (newWidth, newHeight))

    for y in range(orgHeight):
        for x in range(orgWidth):
            xPoint = x + int(y * shearX)
            yPoint = y + int(x * shearY)

            if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                pixel_value = image.getpixel((x, y))
                returnedImage.putpixel((xPoint, yPoint), pixel_value)

    return returnedImage

processedImage = shearImage()
processedImage.save("istanbulSheared.jpg")
processedImage.show()
