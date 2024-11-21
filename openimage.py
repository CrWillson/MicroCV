import numpy as np
import matplotlib.pyplot as plt

def rgb565_to_rgb888(value):
    """Convert a 16-bit RGB565 value to 24-bit RGB888."""
    r = ((value >> 11) & 0x1F) * 255 // 31  # Extract and scale red
    g = ((value >> 5) & 0x3F) * 255 // 63   # Extract and scale green
    b = (value & 0x1F) * 255 // 31          # Extract and scale blue
    return r, g, b

def load_binary_file(file_path):
    """Load space-separated binary strings from a text file and convert to 16-bit integers."""
    with open(file_path, 'r') as f:
        # Read the entire content and split by spaces to get individual binary strings
        binary_strings = f.read().strip().split()
        return [int(binary_str, 2) for binary_str in binary_strings]

def create_image(binary_values, width, height):
    """Convert binary values to an RGB image."""
    if len(binary_values) != width * height:
        raise ValueError("Number of binary values does not match the image dimensions.")
    
    # Convert each RGB565 value to RGB888
    rgb_values = [rgb565_to_rgb888(value) for value in binary_values]
    
    # Reshape to 96x96 and normalize to [0, 1] for display
    image = np.array(rgb_values, dtype=np.uint8).reshape((height, width, 3))
    return image

def main():
    # Path to the file containing 16-bit binary strings
    file_path = 'test.txt'
    
    # Load binary values from file
    binary_values = load_binary_file(file_path)
    
    # Create the 96x96 RGB image
    width, height = 96, 96
    image = create_image(binary_values, width, height)
    
    # Display the image
    plt.imshow(image)
    plt.axis('off')  # Hide axes for better visualization
    plt.title("RGB565 to RGB888 Image")
    plt.show()

if __name__ == "__main__":
    main()
