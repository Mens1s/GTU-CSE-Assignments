
# Lane Detection Using Hough Transform

This project demonstrates lane detection using the Hough Transform algorithm. The goal is to process a video to detect road lanes, fill the lane areas with a colored overlay, and save the processed video with lane markings.

## Features

- **Video Processing**: Reads an input video and processes it frame by frame.
- **ROI Masking**: Applies a Region of Interest (ROI) mask to focus on the road area.
- **Edge Detection**: Utilizes Sobel edge detection to highlight lane edges.
- **Hough Transform**: Detects lines that correspond to road lanes.
- **Lane Grouping**: Groups detected lines into left and right lanes based on slope.
- **Temporal Smoothing**: Smoothes the detected lane positions over time for stability.
- **Lane Area Highlighting**: Fills the area between detected lanes with a transparent color for better visualization.
- **Video Output**: Outputs a video with lane markings and overlays.

## Project Structure

- `with_canny_edge_detection.py`: Script for lane detection using Canny edge detection.
- `without_canny_edge_detection.py`: Script for lane detection without Canny edge detection.
- `input_video/`: Folder containing input video files.
- `output_video/`: Folder where the processed videos will be saved.
- You can look at our output videos from presentation! [here](https://docs.google.com/presentation/d/1wBLHDCuKAh2IriPE0TpyWCPoir7dVtTlytihizvtP6w/edit#slide=id.g321f2b824d8_2_43).
## Requirements

- Python 3.x
- OpenCV (`cv2` library)
- NumPy
- Pillow (`PIL` library)

To install the required dependencies, you can use the following command:

```bash
pip install -r requirements.txt
```

## How to Use

1. **Prepare Your Video File**: Place your input video file in the `input_video/` folder.
2. **Run the Script**: Execute one of the following scripts to process the video:

```bash
python with_canny_edge_detection.py
```

or

```bash
python without_canny_edge_detection.py
```

3. **Check the Output**: The processed video with lane detection will be saved in the `output_video/` folder.

## Workflow

1. **Video Loading**: The script loads the input video using OpenCV.
2. **Frame Processing**: Each frame is converted to grayscale, and a Region of Interest (ROI) mask is applied to focus on the road.
3. **Edge Detection**: Sobel edge detection highlights lane edges. If using the Canny method, it is applied in `with_canny_edge_detection.py`.
4. **Line Detection**: Hough Transform detects straight lines, which are likely to be road lanes.
5. **Lane Grouping**: Detected lines are grouped into left and right lanes based on their slopes.
6. **Smoothing**: Temporal smoothing is applied to detected lanes to reduce noise.
7. **Lane Filling**: The area between detected lanes is filled with a transparent color to visualize the lanes.
8. **Video Output**: The processed frame is saved to the output video file.

## Presentation

You can find a detailed presentation of this project [here](https://docs.google.com/presentation/d/1wBLHDCuKAh2IriPE0TpyWCPoir7dVtTlytihizvtP6w/edit#slide=id.g321f2b824d8_2_43).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
