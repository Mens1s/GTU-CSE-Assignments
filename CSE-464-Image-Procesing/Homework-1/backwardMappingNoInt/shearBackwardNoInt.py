from PIL import Image

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

            if 0 <= xPointOld < orgWidth and 0 <= yPointOld < orgHeight:
                pixel_value = image.getpixel((xPointOld, yPointOld))
                returnedImage.putpixel((x, y), pixel_value)

    return returnedImage

processedImage = shearImage()
processedImage.save("istanbulSheared.jpg")
processedImage.show()
