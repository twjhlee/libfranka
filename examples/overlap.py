import cv2
import os
import pyrealsense2 as rs
import numpy as np
import argparse


def main(args):
    # first move to first pose
    cmd = "./go_to_first_pose 172.16.0.2"
    os.system(cmd)

    # obtain the color image
    overlap_img = cv2.imread(args.overlap_fname)

    # float the two images
    pipeline = rs.pipeline()
    config = rs.config()
    pipeline_wrapper = rs.pipeline_wrapper(pipeline)
    pipeline_profile = config.resolve(pipeline_wrapper)
    device = pipeline_profile.get_device()
    device_product_line = str(device.get_info(rs.camera_info.product_line))

    found_rgb = False
    for s in device.sensors:
        if s.get_info(rs.camera_info.name) == 'RGB Camera':
            found_rgb = True
            break
    if not found_rgb:
        print("The demo requires Depth camera with Color sensor")
        exit(0)

    config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

    if device_product_line == 'L500':
        config.enable_stream(rs.stream.color, 960, 540, rs.format.bgr8, 30)
    else:
        config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

    # Start streaming
    profile = pipeline.start(config)

    # Create an align object
    # rs.align allows us to perform alignment of depth frames to others frames
    # The "align_to" is the stream type to which we plan to align depth frames.
    align_to = rs.stream.color
    align = rs.align(align_to)

    try:
        while True:
            # Get frameset of color and depth
            frames = pipeline.wait_for_frames()
            # frames.get_depth_frame() is a 640x360 depth image

            # Align the depth frame to color frame
            aligned_frames = align.process(frames)

            # Get aligned frames
            color_frame = aligned_frames.get_color_frame()

            # Validate that both frames are valid
            if not color_frame:
                continue

            color_image = np.asanyarray(color_frame.get_data())
            if overlap_img is not None:
                color_image = cv2.addWeighted(color_image, 0.5, overlap_img, 0.5, 0)

            cv2.namedWindow('Align Example', cv2.WINDOW_NORMAL)
            cv2.imshow('Align Example', color_image)
            key = cv2.waitKey(1)
            # Press esc or 'q' to close the image window
            if key & 0xFF == ord('q') or key == 27:
                cv2.destroyAllWindows()
                break
            elif key & 0xFF == ord('c') and overlap_img is None:
                overlap_img = color_image
                cv2.imwrite('overlap.png', overlap_img)
    finally:
        pipeline.stop()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("overlap_fname")
    args = parser.parse_args()

    main(args) 