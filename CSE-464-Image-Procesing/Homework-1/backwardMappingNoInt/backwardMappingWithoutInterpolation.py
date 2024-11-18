from PIL import Image
import math

class BackwardMapping:
    def __init__(self):
        self.image = Image.open("./istanbul.jpg")

    def rotateImage(self, rotateDegrees=60):
        angleRad = rotateDegrees * math.pi / 180

        orgWidth, orgHeight = self.image.size

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

                if 0 <= xOldImagePoint < orgWidth and 0 <= yOldImagePoint < orgHeight:
                    pixel_value = self.image.getpixel((xOldImagePoint, yOldImagePoint))
                    newImageData[y][x] = list(pixel_value)

        for y in range(newHeight):
            for x in range(newWidth):
                returnedImage.putpixel((x, y), tuple(newImageData[y][x]))

        self.image = returnedImage
    
    def scaleImage(self, scaleX=2, scaleY=2):
        orgWidth, orgHeight = self.image.size

        newWidth = int(orgWidth * scaleX)
        newHeight = int(orgHeight * scaleY)
        
        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(newHeight):
            for x in range(newWidth):
                xPointOrg = int(x / scaleX)
                yPointOrg = int(y / scaleY)

                if 0 <= xPointOrg < orgWidth and 0 <= yPointOrg < orgHeight:
                    pixel_value = self.image.getpixel((xPointOrg, yPointOrg))
                    returnedImage.putpixel((x, y), pixel_value)

        self.image = returnedImage
    
    def shearImage(self, shearX=1.4, shearY=0):
        orgWidth, orgHeight = self.image.size
 
        newWidth  = int(orgHeight * shearX) + orgWidth
        newHeight = int(orgWidth * shearY)+ orgHeight

        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(newHeight):
            for x in range(newWidth):
                xPointOld = x - int(y * shearX)
                yPointOld = y - int(x * shearY)

                if 0 <= xPointOld < orgWidth and 0 <= yPointOld < orgHeight:
                    pixel_value = self.image.getpixel((xPointOld, yPointOld))
                    returnedImage.putpixel((x, y), pixel_value)

        self.image = returnedImage
    
    def zoomImage(self, zoom=1.4):
        orgWidth, orgHeight = self.image.size
    
        newWidth  = int(orgWidth * zoom)
        newHeight = int(orgHeight * zoom)

        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(newHeight):
            for x in range(newWidth):
                xPointOld = int(x / zoom)
                yPointOld = int(y / zoom)

                if 0 <= xPointOld < newWidth and 0 <= yPointOld < newHeight:
                    pixel_value = self.image.getpixel((xPointOld, yPointOld))
                    returnedImage.putpixel((x, y), pixel_value)

        self.image = returnedImage

    def getImage(self):
        return self.image
    
backwardMapping = BackwardMapping()

backwardMapping.scaleImage()
backwardMapping.rotateImage()
backwardMapping.shearImage()
backwardMapping.zoomImage()
processedImage = backwardMapping.getImage()
processedImage.save("istanbulAll.jpg")
processedImage.show()
