import numpy as np
from PIL import Image, ImageDraw
import cv2
import os
import math

def preprocess_image(image_array):
    """
    Preprocess the image by applying ROI mask and enhancing contrast.
    """
    height, width = image_array.shape
    
    # Create ROI mask
    mask = np.zeros_like(image_array)
    roi_vertices = np.array([
        [width * 0.1, height],
        [width * 0.4, height * 0.6],
        [width * 0.6, height * 0.6],
        [width * 0.9, height]
    ], dtype=np.int32)
    
    # Fill polygon
    roi_vertices = roi_vertices.reshape((-1, 1, 2))
    cv_mask = Image.new('L', (width, height), 0)
    ImageDraw.Draw(cv_mask).polygon([tuple(x) for x in roi_vertices.reshape(-1, 2)], fill=255)
    mask = np.array(cv_mask)
    
    # Apply mask and enhance contrast
    roi = image_array * (mask / 255)
    p2, p98 = np.percentile(roi[roi > 0], (2, 98))
    processed = np.clip((roi - p2) * (255.0 / (p98 - p2)), 0, 255).astype(np.uint8)
    
    return processed

def edge_detection(image_array):
    """
    Edge detection using enhanced Sobel operator.
    """
    # Gaussian blur
    kernel_size = 5
    kernel = np.ones((kernel_size, kernel_size)) / (kernel_size * kernel_size)
    padded = np.pad(image_array, kernel_size//2, mode='edge')
    blurred = np.zeros_like(image_array, dtype=float)
    
    for i in range(image_array.shape[0]):
        for j in range(image_array.shape[1]):
            window = padded[i:i+kernel_size, j:j+kernel_size]
            blurred[i, j] = np.sum(window * kernel)
    
    # Sobel operators
    kernel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    kernel_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

    gradient_x = np.convolve(blurred.flatten(), kernel_x.flatten(), mode='same').reshape(image_array.shape)
    gradient_y = np.convolve(blurred.flatten(), kernel_y.flatten(), mode='same').reshape(image_array.shape)

    gradient_magnitude = np.sqrt(gradient_x**2 + gradient_y**2)
    
    # Dynamic thresholding
    threshold = np.mean(gradient_magnitude) + np.std(gradient_magnitude)
    edges = (gradient_magnitude > threshold).astype(np.uint8) * 255
    
    return edges

def hough_transform(edge_image, theta_res=1, rho_res=1):
    """
    Hough transform for line detection with road-specific optimizations.
    """
    h, w = edge_image.shape
    diagonal = int(np.sqrt(h**2 + w**2))
    rhos = np.arange(-diagonal, diagonal, rho_res)
    
    # Road-specific angles
    thetas = np.deg2rad(np.concatenate([
        np.arange(20, 80, theta_res),
        np.arange(100, 160, theta_res)
    ]))

    accumulator = np.zeros((len(rhos), len(thetas)), dtype=np.int32)
    edge_points = np.argwhere(edge_image > 0)

    for y, x in edge_points:
        for theta_idx, theta in enumerate(thetas):
            rho = int(x * np.cos(theta) + y * np.sin(theta))
            rho_idx = np.argmin(np.abs(rhos - rho))
            accumulator[rho_idx, theta_idx] += 1

    threshold = np.max(accumulator) * 0.3
    detected_lines = []
    for rho_idx, theta_idx in np.argwhere(accumulator > threshold):
        rho = rhos[rho_idx]
        theta = thetas[theta_idx]
        detected_lines.append((rho, theta))

    return detected_lines

def group_lines(lines, width, height):
    """
    Group detected lines into left and right lanes.
    """
    left_lines = []
    right_lines = []

    for rho, theta in lines:
        a, b = np.cos(theta), np.sin(theta)
        x0, y0 = a * rho, b * rho
        y1, y2 = height, int(height * 0.6)
        x1 = int((rho - y1 * b) / a)
        x2 = int((rho - y2 * b) / a)

        slope = (y2 - y1) / (x2 - x1 + 1e-6)
        
        if -0.9 < slope < -0.4:
            left_lines.append(((x1, y1), (x2, y2)))
        elif 0.4 < slope < 0.9:
            right_lines.append(((x1, y1), (x2, y2)))

    def robust_average_line(lines):
        if len(lines) == 0:
            return None
        
        points = np.array([line[0] + line[1] for line in lines])
        x1s, y1s, x2s, y2s = points.T
        
        def reject_outliers(data):
            median = np.median(data)
            mad = np.median(np.abs(data - median))
            modified_zscore = 0.6745 * (data - median) / mad
            return data[np.abs(modified_zscore) < 2.5]
        
        x1s = reject_outliers(x1s)
        x2s = reject_outliers(x2s)
        
        if len(x1s) == 0 or len(x2s) == 0:
            return None
        
        return (int(np.mean(x1s)), height), (int(np.mean(x2s)), int(height * 0.6))

    left_lane = robust_average_line(left_lines)
    right_lane = robust_average_line(right_lines)

    return left_lane, right_lane

def fill_between_lanes(frame, left_lane, right_lane, height, color=(0, 255, 0, 80)):
    """
    Fill the detected lane area with a transparent color.
    """
    if not left_lane or not right_lane:
        return

    overlay = frame.copy()
    points = [
        left_lane[0], left_lane[1],
        right_lane[1], right_lane[0]
    ]
    
    cv2.fillPoly(overlay, [np.array(points, dtype=np.int32)], color)

    # Add transparency to the overlay
    cv2.addWeighted(overlay, 0.3, frame, 0.7, 0, frame)

def process_video(input_path, output_path):
    """
    Process video file and detect lanes using cv2 for video reading.
    """
    # Open video file
    cap = cv2.VideoCapture(input_path)
    
    if not cap.isOpened():
        raise ValueError("Error opening video file")
    
    # Get video properties
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = int(cap.get(cv2.CAP_PROP_FPS))
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    
    # Calculate frame interval for 1 frame per second
    frame_interval = fps
    
    # Create video writer
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(output_path, fourcc, fps, (width, height))
    
    # Buffers for temporal smoothing
    left_lanes_buffer = []
    right_lanes_buffer = []
    buffer_size = 5

    frame_idx = 0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # Process only frames at the specified interval
        if frame_idx % frame_interval != 0:
            frame_idx += 1
            continue

        # Convert to grayscale
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # Process frame
        processed = preprocess_image(gray_frame)
        edges = edge_detection(processed)
        lines = hough_transform(edges)
        left_lane, right_lane = group_lines(lines, width, height)
        
        # Temporal smoothing
        if left_lane:
            left_lanes_buffer.append(left_lane)
        if right_lane:
            right_lanes_buffer.append(right_lane)
            
        if len(left_lanes_buffer) > buffer_size:
            left_lanes_buffer.pop(0)
        if len(right_lanes_buffer) > buffer_size:
            right_lanes_buffer.pop(0)
        
        smoothed_left = None
        smoothed_right = None
        
        if left_lanes_buffer:
            points = np.array(left_lanes_buffer)
            smoothed_left = tuple(map(tuple, np.mean(points, axis=0).astype(int)))
        if right_lanes_buffer:
            points = np.array(right_lanes_buffer)
            smoothed_right = tuple(map(tuple, np.mean(points, axis=0).astype(int)))

        # Draw lanes directly on the original frame
        frame_with_overlay = frame.copy()
        fill_between_lanes(frame_with_overlay, smoothed_left, smoothed_right, height)

        # Write frame
        for _ in range(fps):
            out.write(frame_with_overlay)

        frame_idx += 1
        print(f"Processed frame {frame_idx}/{total_frames}", end='\r')
    # Release resources
    cap.release()
    out.release()
    print(f"Processed video saved to {output_path}")

def main():
    """
    Main function to demonstrate lane detection.
    """
    input_video_path = "america_test.mp4"  # Replace with your video path
    output_video_path = "america_test_output.mp4"
    
    if not os.path.exists(input_video_path):
        raise FileNotFoundError(f"Input video not found at path: {input_video_path}")
    
    # Process the video
    print("Processing video...")
    process_video(input_video_path, output_video_path)
    print("Done! Check output_lane_detection.mp4 for results.")

if __name__ == "__main__":
    main()
