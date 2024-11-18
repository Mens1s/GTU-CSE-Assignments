from PIL import Image

def zoomImage(zoom = 1.4):
    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size
 
    newWidth  = int(orgWidth * zoom)
    newHeight = int(orgHeight * zoom)

    returnedImage = Image.new("RGB", (newWidth, newHeight))

    for y in range(orgHeight):
        for x in range(orgWidth):
            xPoint = int(x * zoom)
            yPoint = int(y * zoom)

            if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                pixel_value = image.getpixel((x, y))
                returnedImage.putpixel((xPoint, yPoint), pixel_value)

    return returnedImage

processedImage = zoomImage()
processedImage.save("istanbulZoomed.jpg")
processedImage.show()
