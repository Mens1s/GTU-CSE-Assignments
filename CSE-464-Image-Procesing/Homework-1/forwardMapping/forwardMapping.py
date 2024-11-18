from PIL import Image
import math

class ForwardMapping:
    def __init__(self):
        self.image = Image.open("./istanbul.jpg")

    def shearImage(self, shearX=1.4, shearY=0):
        orgWidth, orgHeight = self.image.size
    
        newWidth  = int(orgHeight * shearX) + orgWidth
        newHeight = int(orgWidth * shearY)+ orgHeight

        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(orgHeight):
            for x in range(orgWidth):
                xPoint = x + int(y * shearX)
                yPoint = y + int(x * shearY)

                if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                    pixel_value = self.image.getpixel((x, y))
                    returnedImage.putpixel((xPoint, yPoint), pixel_value)

        self.image = returnedImage


    def rotateImage(self, rotateDegrees=60):
        angleRad = rotateDegrees * math.pi / 180

        orgWidth, orgHeight = self.image.size

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
                    pixel_value = self.image.getpixel((x, y))
                    newImageData[yPoint][xPoint] = list(pixel_value)

        for y in range(newHeight):
            for x in range(newWidth):
                returnedImage.putpixel((x, y), tuple(newImageData[y][x]))

        self.image = returnedImage
    
    def scaleImage(self, scaleX=2, scaleY=2):
        orgWidth, orgHeight = self.image.size

        newWidth = int(orgWidth * scaleX)
        newHeight = int(orgHeight * scaleY)
        
        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(orgHeight):
            for x in range(orgWidth):
                xPoint = int(x * scaleX)
                yPoint = int(y * scaleY)

                if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                    pixel_value = self.image.getpixel((x, y))
                    returnedImage.putpixel((xPoint, yPoint), pixel_value)

        self.image = returnedImage
    
    def zoomImage(self, zoom = 1.4):
        orgWidth, orgHeight = self.image.size
    
        newWidth  = int(orgWidth * zoom)
        newHeight = int(orgHeight * zoom)

        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(orgHeight):
            for x in range(orgWidth):
                xPoint = int(x * zoom)
                yPoint = int(y * zoom)

                if 0 <= xPoint < newWidth and 0 <= yPoint < newHeight:
                    pixel_value = self.image.getpixel((x, y))
                    returnedImage.putpixel((xPoint, yPoint), pixel_value)

        self.image = returnedImage

    def getImage(self):
        return self.image
    
forwardMapping = ForwardMapping()

forwardMapping.scaleImage()
forwardMapping.rotateImage()
forwardMapping.shearImage()
forwardMapping.zoomImage()
processedImage = forwardMapping.getImage()
processedImage.save("istanbulAll.jpg")
processedImage.show()
