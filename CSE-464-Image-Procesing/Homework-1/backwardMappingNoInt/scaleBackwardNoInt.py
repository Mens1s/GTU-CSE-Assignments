from PIL import Image

def scaleImage(scaleX=2, scaleY=2):
    image = Image.open("./istanbul.jpg")

    orgWidth, orgHeight = image.size

    newWidth = int(orgWidth * scaleX)
    newHeight = int(orgHeight * scaleY)
    
    returnedImage = Image.new("RGB", (newWidth, newHeight))

    for y in range(newHeight):
        for x in range(newWidth):
            xPointOrg = int(x / scaleX)
            yPointOrg = int(y / scaleY)

            if 0 <= xPointOrg < orgWidth and 0 <= yPointOrg < orgHeight:
                pixel_value = image.getpixel((xPointOrg, yPointOrg))
                returnedImage.putpixel((x, y), pixel_value)

    return returnedImage

processedImage = scaleImage()
processedImage.save("istanbulScaled.jpg")
processedImage.show()
