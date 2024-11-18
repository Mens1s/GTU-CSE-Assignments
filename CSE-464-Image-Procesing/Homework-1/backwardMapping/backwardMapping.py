from PIL import Image
import math

class BackwardMapping:
    def __init__(self):
        self.image = Image.open("./istanbul.jpg")

    def bilinearInterpolate(self, x, y):
        x0, y0 = int(math.floor(x)), int(math.floor(y))
        x1, y1 = min(x0 + 1, self.image.width - 1), min(y0 + 1, self.image.height - 1)

        if x0 < 0 or y0 < 0 or x0 >= self.image.width or y0 >= self.image.height:
            return (0, 0, 0)

        # q11      q12
        #    TARGET
        # q21      q22

        q11 = self.image.getpixel((x0, y0))
        q21 = self.image.getpixel((x1, y0))
        q12 = self.image.getpixel((x0, y1))
        q22 = self.image.getpixel((x1, y1))

        f_x1 = x - x0 # yüzdelik alma yeri hocanın derste ____0.4X____TARGET____0.6X_____ dedigi yer
        f_y1 = y - y0 # yüzdelik alma yeri hocanın derste ____0.4Y____TARGET____0.6Y_____ dedigi yer

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

                pixel_value = self.bilinearInterpolate(xOldImagePoint, yOldImagePoint)
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
                xPointOrg = (x / scaleX)
                yPointOrg = (y / scaleY)

                pixel_value = self.bilinearInterpolate( xPointOrg, yPointOrg)
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

                pixel_value = self.bilinearInterpolate(xPointOld, yPointOld)
                returnedImage.putpixel((x, y), pixel_value)

        self.image = returnedImage
    
    def zoomImage(self, zoom=1.4):
        orgWidth, orgHeight = self.image.size
        newWidth = int(orgWidth * zoom)
        newHeight = int(orgHeight * zoom)

        returnedImage = Image.new("RGB", (newWidth, newHeight))

        for y in range(newHeight):
            for x in range(newWidth):
                xPointOld = x / zoom
                yPointOld = y / zoom

                pixel_value = self.bilinearInterpolate(xPointOld, yPointOld)

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
