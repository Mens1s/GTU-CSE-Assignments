import cv2
import numpy as np
import matplotlib.pyplot as plt
import math
from numpy import array, zeros
from numpy import max as npmax
import cv2
import cv2
from numpy import array, zeros
from PIL import Image
from matplotlib.pyplot import imshow, show, subplot, gray, title, axis, figure, text

"""
    Way-Detection Algorithm: 
    1-)Convert the Image to Grayscale: Transform the color image into a single-channel grayscale image.
    2-)Apply Gaussian Blur: Reduce noise in the image using a Gaussian blur.
    3-)Calculate Gradients: Determine where the intensity changes significantly using Sobel filters.
    4-)Non-Maximum Suppression: Thin out the detected edges by keeping only local maxima in the gradient magnitude.
    5-)Double Thresholding: Classify pixels as strong edges or weak edges based on their gradient values.
    6-)Final Edge Selection: Perform edge tracking by hysteresis to finalize which weak edges should be retained
       based on their connectivity to strong edges.

"""
def rgb_to_grayscale(image):
    # Get image dimensions
    height, width = image.shape[0], image.shape[1]
    
    # Create an empty array for the grayscale image
    grayscale = [[0 for _ in range(width)] for _ in range(height)]
    
    # Convert each pixel to grayscale
    for i in range(height):
        for j in range(width):
            # Extract RGB values
            r = image[i][j][0]
            g = image[i][j][1]
            b = image[i][j][2]
            
            # Calculate grayscale value using standard weights
            # Gray = 0.299R + 0.587G + 0.114B
            gray_value = (0.299 * r + 0.587 * g + 0.114 * b)
            
            # Ensure the value is in valid range [0, 255]
            gray_value = min(max(0, gray_value), 255)
            
            # Store the grayscale value
            grayscale[i][j] = int(gray_value)
    
    # Convert the grayscale list to a numpy array
    return np.array(grayscale, dtype=np.uint8)

def create_gaussian_kernel(size, sigma):

    # Ensure size is odd
    if size % 2 == 0:
        size += 1
    
    # Calculate center point
    center = size // 2
    
    # Create empty kernel
    kernel = [[0 for _ in range(size)] for _ in range(size)]
    
    # Calculate sum for normalization
    sum_vals = 0
    
    # Fill the kernel with Gaussian values
    for i in range(size):
        for j in range(size):
            # Calculate x and y distances from center
            x = i - center
            y = j - center
            
            # Gaussian function
            exponent = -(x**2 + y**2)/(2*sigma**2)
            gaussian_val = (1/(2*math.pi*sigma**2)) * math.exp(exponent)
            
            kernel[i][j] = gaussian_val
            sum_vals += gaussian_val
    
    # Normalize the kernel
    for i in range(size):
        for j in range(size):
            kernel[i][j] /= sum_vals
    
    return kernel

def apply_gaussian_blur_grayscale(grayscale_image, kernel_size=3, sigma=1.0):
    # Create gaussian kernel
    kernel = create_gaussian_kernel(kernel_size, sigma)
    
    # Get image dimensions
    height = len(grayscale_image)
    width = len(grayscale_image[0])
    
    # Calculate padding size
    pad = kernel_size // 2
    
    # Create output image
    output = [[0 for _ in range(width)] for _ in range(height)]
    
    # Apply convolution
    for i in range(height):
        for j in range(width):
            value = 0
            for ki in range(kernel_size):
                for kj in range(kernel_size):
                    # Calculate image coordinates with padding
                    im_i = i - pad + ki
                    im_j = j - pad + kj
                    
                    # Handle boundaries by mirroring
                    if im_i < 0: im_i = abs(im_i)
                    if im_j < 0: im_j = abs(im_j)
                    if im_i >= height: im_i = 2*height - im_i - 2
                    if im_j >= width: im_j = 2*width - im_j - 2
                    
                    value += grayscale_image[im_i][im_j] * kernel[ki][kj]
            
            output[i][j] = int(max(0, min(255, value)))
    
    return np.array(output, dtype=np.uint8)

class edge_tracking:
    def __init__(self, tr):
        self.img = tr[0]
        self.strongs = tr[1]
        self.shape = self.img.shape
        self.vis = zeros(self.shape, bool)
        self.dx = [1, 0, -1,  0, -1, -1, 1,  1]
        self.dy = [0, 1,  0, -1,  1, -1, 1, -1]
        
        for s in self.strongs:
            if not self.vis[s]:
                self.dfs(s)
        
        for i in range(self.shape[0]):
            for j in range(self.shape[1]):
                self.img[i, j] = 1.0 if self.vis[i, j] else 0.0

    def dfs(self, origin):
        q = [origin]
        while len(q) > 0:
            s = q.pop()
            self.vis[s] = True
            self.img[s] = 1
            for k in range(len(self.dx)):
                for c in range(1, 16):
                    nx, ny = s[0] + c * self.dx[k], s[1] + c * self.dy[k]
                    if self.exists(nx, ny) and (self.img[nx, ny] >= 0.5) and (not self.vis[nx, ny]):
                        q.append((nx, ny))

    def exists(self, x, y):
        return x >= 0 and x < self.shape[0] and y >= 0 and y < self.shape[1]

    def get_result(self):
        return self.img


def double_thresholding(im):
    thres  = zeros(im.shape)
    strong = 1
    weak   = 0.6
    mmax = npmax(im)
    lo, hi = 0.1 * mmax,0.8 * mmax
    
    strongs = []
    for i in range(im.shape[0]):
        for j in range(im.shape[1]):
            px = im[i][j]
            if px >= hi:
                thres[i][j] = strong
                strongs.append((i, j))
            elif px >= lo:
                thres[i][j] = weak
    return thres, strongs


from numpy import array, zeros, abs
from numpy.fft import fft2, ifft2

def gaussianfilter(im):
    b = array([[2, 4,  5,  2,  2],
               [4, 9,  12, 9,  4],
               [5, 12, 15, 12, 5],
               [4, 9,  12, 9,  4],
               [2, 4,  5,  4,  2]]) / 156
    kernel = zeros(im.shape)
    kernel[:b.shape[0], :b.shape[1]] = b

    fim = fft2(im)
    fkernel = fft2(kernel)
    fil_im = ifft2(fim * fkernel)

    return abs(fil_im).astype(int)

from numpy import array, zeros, abs, sqrt, arctan2, arctan, pi, real
from numpy.fft import fft2, ifft2


def findinggradient(im):
    op1 = array([[-1, 0, 1],
                 [-2, 0, 2],
                 [-1, 0, 1]])
    op2 = array([[-1, -2, -1],
                 [ 0,  0,  0],
                 [ 1,  2,  1]])
    kernel1 = zeros(im.shape)
    kernel1[:op1.shape[0], :op1.shape[1]] = op1
    kernel1 = fft2(kernel1)

    kernel2 = zeros(im.shape)
    kernel2[:op2.shape[0], :op2.shape[1]] = op2
    kernel2 = fft2(kernel2)

    fim = fft2(im)
    Gx = real(ifft2(kernel1 * fim)).astype(float)
    Gy = real(ifft2(kernel2 * fim)).astype(float)

    G = sqrt(Gx**2 + Gy**2)
    Theta = arctan2(Gy, Gx) * 180 / pi
    return G, Theta

from numpy import array, zeros
def nonmax_suppression(det, phase):
    gmax = zeros(det.shape)
    height, width = det.shape
    
    for i in range(height):
        for j in range(width):
            # Normalize angle to be between 0 and 360 degrees
            angle = phase[i][j]
            if angle < 0:
                angle += 360
            
            # Interpolate between neighboring pixels for more precise edge detection
            try:
                # Determine the two neighboring pixels based on gradient direction
                if (angle >= 337.5 or angle < 22.5) or (angle >= 157.5 and angle < 202.5):
                    # Horizontal direction
                    neighbors = [det[i][npmax(0, j-1)], det[i][min(width-1, j+1)]]
                elif (angle >= 22.5 and angle < 67.5) or (angle >= 202.5 and angle < 247.5):
                    # 45-degree direction
                    neighbors = [det[npmax(0, i-1)][min(width-1, j+1)], det[min(height-1, i+1)][npmax(0, j-1)]]
                elif (angle >= 67.5 and angle < 112.5) or (angle >= 247.5 and angle < 292.5):
                    # Vertical direction
                    neighbors = [det[npmax(0, i-1)][j], det[min(height-1, i+1)][j]]
                else:  # (angle >= 112.5 and angle < 157.5) or (angle >= 292.5 and angle < 337.5)
                    # 135-degree direction
                    neighbors = [det[npmax(0, i-1)][npmax(0, j-1)], det[min(height-1, i+1)][min(width-1, j+1)]]
                
                # Check if current pixel is a local maximum
                if det[i][j] >= npmax(neighbors):
                    gmax[i][j] = det[i][j]
                else:
                    gmax[i][j] = 0
            
            except IndexError:
                # Fallback for edge cases
                gmax[i][j] = det[i][j]
    
    return gmax


def fill_poly_without_cv2(mask, polygon, color):
    """Verilen poligonun içinde kalan bölgeyi doldurur."""
    # Poligonun köşe noktalarını ayır
    polygon = polygon.reshape((-1, 2))
    
    # Maskenin boyutları
    height, width = mask.shape
    
    # Poligonun içerdiği her pikseli kontrol et
    for y in range(height):
        for x in range(width):
            # Pikselin poligon içinde olup olmadığını kontrol et
            if is_point_in_polygon(x, y, polygon):
                mask[y, x] = color
    return mask

def is_point_in_polygon(x, y, polygon):
    """Bir noktanın poligon içinde olup olmadığını kontrol eder."""
    # Ray-casting algoritması kullanılarak poligon içinde mi diye kontrol eder
    n = len(polygon)
    inside = False
    p1x, p1y = polygon[0]
    for i in range(n + 1):
        p2x, p2y = polygon[i % n]
        if y > min(p1y, p2y):
            if y <= max(p1y, p2y):
                if x <= max(p1x, p2x):
                    if p1y != p2y:
                        xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                    if p1x == p2x or x <= xinters:
                        inside = not inside
        p1x, p1y = p2x, p2y
    return inside

def region_of_interest(image):
    """ROI belirler ve sadece ilgili bölgeyi maskele."""
    height, width = image.shape[:2]
    polygon = np.array([[
        (width * 0.1, height),  # Sol alt
        (width * 0.4, height * 0.6),  # Sol üst
        (width * 0.6, height * 0.6),  # Sağ üst
        (width * 0.9, height)  # Sağ alt
    ]], dtype=np.int32)
    
    # Mask oluştur ve uint8'e dönüştür
    mask = np.zeros_like(image, dtype=np.uint8)
    mask = fill_poly_without_cv2(mask, polygon, 255)  # fill_poly_without_cv2'nin uint8 ile çalıştığından emin olun
    
    # Görüntüyü uint8'e dönüştür
    if image.dtype != np.uint8:
        image = (image * 255).astype(np.uint8) if image.max() <= 1 else image.astype(np.uint8)
    
    # Bitwise AND işlemi
    masked_image = np.bitwise_and(image, mask)
    return masked_image

import numpy as np


def detect_lines(image):
    """Hough Line Transform kullanarak şeritleri bul."""
    lines = cv2.HoughLinesP(image, 1, np.pi / 180, threshold=50, minLineLength=50, maxLineGap=150)
    return lines


def detect_lanes(image):
    # Convert image to grayscale
    my_gray = rgb_to_grayscale(image)

    # Apply Gaussian Blur to reduce noise and smooth edges
    blur_my = apply_gaussian_blur_grayscale(my_gray, 5, sigma=1.0)

    # Edge detection process
    grim, gphase = findinggradient(blur_my)
    gmax = nonmax_suppression(grim, gphase)
    thres, strongs = double_thresholding(gmax)
    edge_tracker = edge_tracking((thres, strongs))
    edges = edge_tracker.get_result()
    
    roi_edges = region_of_interest(edges)
    lines = detect_lines(roi_edges)
    
    # Create a blank image to draw the lines
    line_image = np.zeros_like(image)
    
    # Separate left and right lane lines
    left_lines = []
    right_lines = []
    
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line[0]
            # Calculate slope to separate left and right lines
            slope = (y2 - y1) / (x2 - x1)
            if slope < -0.5:  # Left lane
                left_lines.append(line)
            elif slope > 0.5:  # Right lane
                right_lines.append(line)
                
        # Average the positions of the left and right lines to get a single line for each lane
        def average_line(lines):
            x_coords = []
            y_coords = []
            for line in lines:
                x1, y1, x2, y2 = line[0]
                x_coords += [x1, x2]
                y_coords += [y1, y2]
            if x_coords and y_coords:
                polyfit = np.polyfit(y_coords, x_coords, deg=1)
                y1 = image.shape[0]
                y2 = int(y1 * 0.65)
                x1 = int(np.polyval(polyfit, y1))
                x2 = int(np.polyval(polyfit, y2))
                return [[x1, y1, x2, y2]]
            else:
                return None

        # Draw averaged lane lines on the blank image
        left_lane = average_line(left_lines)
        right_lane = average_line(right_lines)
        
        if left_lane is not None:
            cv2.line(line_image, (left_lane[0][0], left_lane[0][1]), (left_lane[0][2], left_lane[0][3]), (0, 255, 0), 10)
        if right_lane is not None:
            cv2.line(line_image, (right_lane[0][0], right_lane[0][1]), (right_lane[0][2], right_lane[0][3]), (0, 255, 0), 10)
        
        # Fill the polygon area between the lanes with green
        if left_lane is not None and right_lane is not None:
            lane_polygon = np.array([[
                (left_lane[0][0], left_lane[0][1]),
                (left_lane[0][2], left_lane[0][3]),
                (right_lane[0][2], right_lane[0][3]),
                (right_lane[0][0], right_lane[0][1])
            ]], np.int32)
            cv2.fillPoly(line_image, lane_polygon, (0, 255, 0))
    
    # Combine the line image with the original image
    output_image = cv2.addWeighted(image, 0.8, line_image, 0.2, 1)
    
    return output_image

# # Load a sample road image
# image = cv2.imread('test3.jpg')  # Replace with your actual image path
# detect_lanes(image)


def process_video(input_video_path, output_video_path):
    # Open the input video
    cap = cv2.VideoCapture(input_video_path)
    
    # Get video properties
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    fps = cap.get(cv2.CAP_PROP_FPS)
    total_duration = total_frames / fps
    
    # Create VideoWriter object
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(output_video_path, fourcc, fps, (width, height))
    
    # Calculate frame interval for 1 frame per second
    frame_interval = int(fps)
    
    frame_count = 0
    processed_frames = []
    
    # First pass: collect processed frames
    while cap.isOpened():
        ret, frame = cap.read()
        
        if not ret:
            break
        
        # Process one frame per second
        if frame_count % frame_interval == 0:
            # Apply lane detection
            processed_frame = detect_lanes(frame)
            processed_frames.append(processed_frame)
        
        frame_count += 1
        print("Processed frame:", frame_count)
    
    # Reset video capture
    cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
    
    # Calculate how many times to repeat each processed frame
    frames_per_second_processed = len(processed_frames)
    repeat_count = int(total_frames / frames_per_second_processed)
    
    # Second pass: write frames to output video
    processed_frame_index = 0
    for _ in range(total_frames):
        if (_ % repeat_count == 0):
            current_processed_frame = processed_frames[processed_frame_index]
            processed_frame_index = min(processed_frame_index + 1, len(processed_frames) - 1)
        
        out.write(current_processed_frame)
    
    # Release video objects
    cap.release()
    out.release()
    
    print(f"Input video duration: {total_duration:.2f} seconds")
    print(f"Processed frames: {len(processed_frames)}")
    print(f"Output video duration maintained at {total_duration:.2f} seconds")

# Example usage
input_video_path = 'test_video.mp4'  # Replace with your input video path
output_video_path = 'lane_detection_output.mp4'
process_video(input_video_path, output_video_path)