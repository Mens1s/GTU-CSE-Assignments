from PIL import Image

def scaleImage(scaleX=2, scaleY=2):
    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size

    newWidth = int(orgWidth * scaleX)
    newHeight = int(orgHeight * scaleY)
    
    returnedImage = Image.new("RGB", (newWidth, newHeight))

    for y in range(orgHeight):
        for x in range(orgWidth):
            xPoint = int(x * scaleX)
            yPoint = int(y * scaleY)

            if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                pixel_value = image.getpixel((x, y))
                returnedImage.putpixel((xPoint, yPoint), pixel_value)

    return returnedImage

processedImage = scaleImage()
processedImage.save("istanbulScaled.jpg")
processedImage.show()
