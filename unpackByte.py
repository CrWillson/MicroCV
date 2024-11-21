def unpack_values(packed_byte):
    # Extract the bools from the packed byte
    b1 = (packed_byte >> 6) & 0x01  # Extract the 7th bit
    b2 = (packed_byte >> 7) & 0x01  # Extract the 8th bit

    # Extract the normalized num and revert to the original range
    num = (packed_byte & 0x3F)  # Mask the first 6 bits and adjust the range

    return num, bool(b1), bool(b2)

packed_byte = 0b00011100  # Example packed byte

num, b1, b2 = unpack_values(packed_byte)

print(f"Number: {num}, Bool 1: {b1}, Bool 2: {b2}")