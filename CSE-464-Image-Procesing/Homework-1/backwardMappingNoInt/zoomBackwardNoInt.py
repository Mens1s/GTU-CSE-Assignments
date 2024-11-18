from PIL import Image

def zoomImage(zoom = 1.4):
    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size
 
    newWidth  = int(orgWidth * zoom)
    newHeight = int(orgHeight * zoom)

    returnedImage = Image.new("RGB", (newWidth, newHeight))

    for y in range(newHeight):
        for x in range(newWidth):
            xPointOld = int(x / zoom)
            yPointOld = int(y / zoom)

            if 0 <= xPointOld < newWidth and 0 <= yPointOld < newHeight:
                pixel_value = image.getpixel((xPointOld, yPointOld))
                returnedImage.putpixel((x, y), pixel_value)

    return returnedImage

processedImage = zoomImage()
processedImage.save("istanbulZoomed.jpg")
processedImage.show()
