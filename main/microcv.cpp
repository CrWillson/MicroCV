#include "microcv.hpp"

/// @brief Finds the center of the lane in the image.
/// @param mask The binary image.
/// @param start_row Based upon our cropping, we know that allot of the image won't
/// contain any data. Pass this in, so that we don't waste time considering that
/// sector of the image.
/// @return The project center column.
uint8_t MicroCV::get_lane_center(const cv::Mat1b& mask, const uint8_t start_row)
{
    //const auto start_tick = xTaskGetTickCount();
    uint16_t result = 0; // The center column.
    uint16_t sums[mask.cols] = {0};

    // Sum up the columns into the "sums" array,
    for (uint16_t row = start_row; row < mask.rows; row++) {
        for (uint16_t col = 0; col < mask.cols; col++) {
            sums[col] += mask.at<uint8_t>(row, col);
        }
    }

    // Split the image into two halves -- the left half should contain the left dotted line,
    // the right half should contain the right solid line.
    const uint8_t half = (mask.cols >> 1);

    // Find the max of that which is on the left side of the image. Call that the dotted line.
    uint16_t dotted_col = 0;
    uint16_t max = 0;

    for (uint16_t col = 0; col < half; col++) {
        const uint16_t val = sums[col];
        if (val > max) {
            max = val;
            dotted_col = col;
        }
    }

    // Find the max of that which is on the right side of the image. Call that the solid line.
    uint16_t solid_col = 0;
    max = 0;

    for (uint16_t col = half; col < mask.cols; col++) {
        const uint16_t val = sums[col];
        if (val > max) {
            max = val;
            solid_col = col;
        }
    }

    // The center of the lane is the average of the right and left lines.
    result = ((dotted_col + solid_col) >> 1);

    //const auto end_tick = xTaskGetTickCount();
    //printf("Ticks for get_lane_center: %ld\n", (end_tick - start_tick));
    return result;
}


/// @brief Finds the contour of the solid line, assumed to be the largest contour in the given mask.
/// @param mask The binary image.
/// @return The contour of the solid line.
contour_t MicroCV::get_line(const cv::Mat1b& mask)
{
    // Get the contours.
    std::vector<contour_t> contours;
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    if (0 == contours.size()) {
        return contour_t();
    }

    // Find the largest contour, assume that's the solid line.
    std::sort(contours.begin(), contours.end(), [](const contour_t& a, const contour_t& b) {
        const cv::Rect2i a_rect = cv::boundingRect(a);
        const cv::Rect2i b_rect = cv::boundingRect(b);

        return a_rect.area() > b_rect.area();
    });
    const auto& solid_line = contours[0];
    return solid_line;
}


/// @brief Gets the slope through the solid line.
/// @param contour The contour of the solid line.
/// @return The slope.
float MicroCV::get_slope(const contour_t& contour)
{
    // Find the furthest left and furthest right point.
    cv::Point2i leftmost(INT_MAX, 0);
    cv::Point2i rightmost(INT_MIN, 0);

    for (const auto& point : contour) {
        // Check for leftmost
        if (point.x < leftmost.x) {
            leftmost = point;
        }

        // Check for rightmost
        if (point.x > rightmost.x) {
            rightmost = point;
        }
    }


    // Since we know that the solid line is approximately "line-shaped," an approximation
    // of the slope should be just rise over run with these.
    const float rise = leftmost.y - rightmost.y;
    const float run = leftmost.x - rightmost.x;

    float slope;
    if (run != 0) {
        slope = rise/run;
    } else {
        slope = INFINITY;
    }

    return slope;
}


/// @brief Crops a captured frame based upon cropping parameters.
/// @param frame The frame to crop. Modified.
/// @param top The number of pixels on the top to crop off.
/// @param bottom The number of pixels on the bottom to crop off.
/// @param left The number of pixels on the left to crop off.
/// @param right The number of pixels on the right to crop off.
void MicroCV::apply_cropping(cv::Mat& frame, const uint16_t top, const uint16_t bottom, const uint16_t left, const uint16_t right)
{
    // Do the top cropping.
    if (top > 0) {
        cv::rectangle(frame, cv::Rect2i(cv::Point2i(0, 0), cv::Point2i(frame.cols, top)), cv::Scalar(0, 0, 0), -1);
    }

    // Do the bottom cropping.
    if (bottom > 0) {
        cv::rectangle(frame, cv::Rect2i(cv::Point2i(0, frame.rows), cv::Point2i(frame.cols, frame.rows - bottom)), cv::Scalar(0, 0, 0), -1);
    }

    // Do the left cropping.
    if (left > 0) {
        cv::rectangle(frame, cv::Rect2i(cv::Point2i(0, 0), cv::Point2i(left, frame.rows)), cv::Scalar(0, 0, 0), -1);
    }

    // Do the right cropping.
    if (right > 0) {
        cv::rectangle(frame, cv::Rect2i(cv::Point2i(frame.cols, 0), cv::Point2i(frame.cols - right, frame.rows)), cv::Scalar(0, 0, 0), -1);
    }
}


/// @brief Finds the outside line and extracts parameters.
/// @param hsv The frame, in HSV, to extract data from.
/// @param thresh The thresholded frame. Output param.
/// @param center_point The centerpoint of the detected line. Output param.
/// @param slope The slope of the detected line. Output param.
void MicroCV::outside_line_detection(cv::Mat& hsv, cv::Mat1b& thresh, cv::Point2i& center_point, float& slope)
{
    apply_cropping(hsv, outside_cropping_top, outside_cropping_bottom, outside_cropping_left, outside_cropping_right);

    const auto low = cv::Scalar(outside_thresh_min_hue, outside_thresh_min_sat, outside_thresh_min_val);
    const auto high = cv::Scalar(outside_thresh_max_hue, outside_thresh_max_sat, outside_thresh_max_val);
    cv::inRange(hsv, low, high, thresh);

    #if PRINT_LINE_THRESH == 1
    debug::print_thresh(thresh);
    #endif

    const auto solid_line = get_line(thresh);
    if (0 == solid_line.size()) {
        center_point.x = -1;
        center_point.y = -1;
        slope = NAN;
    } else {
        const auto solid_line_rect = cv::boundingRect(solid_line);

        // If there is less area than the min. expected, don't record as a detection
        if (solid_line_rect.area() < outside_min_detect_area) {
            center_point.x = -1;
            center_point.y = -1;
            slope = NAN;
        }

        center_point.x = solid_line_rect.x + (solid_line_rect.width >> 1);
        center_point.y = solid_line_rect.y + (solid_line_rect.height >> 1);

        cv::line(thresh, cv::Point2i(center_point.x, 0), cv::Point2i(center_point.x, thresh.rows), 0xff);

        slope = get_slope(solid_line);
    }
}


bool MicroCV::rectangles_overlap(cv::Rect2i rect1, cv::Rect2i rect2)
{
    // Check if there is no overlap between the two rectangles
    return !(rect1.x > rect2.x + rect2.width || rect2.x > rect1.x + rect1.width ||
        rect1.y > rect2.y + rect2.height || rect2.y > rect1.y + rect1.height);
}


/// @brief Finds the red line and extracts parameters.
/// @param hsv The frame, in HSV, to extract data from.
/// @param thresh The threshold frame, Output param.
/// @param detected Whether or not the red line is "detected." Output param.
void MicroCV::stop_line_detection(cv::Mat& hsv, cv::Mat1b& thresh, bool& detected)
{
    apply_cropping(hsv, stop_cropping_top, stop_cropping_bottom, stop_cropping_left, stop_cropping_right);

    const auto low = cv::Scalar(stop_thresh_min_hue, stop_thresh_min_sat, stop_thresh_min_val);
    const auto high = cv::Scalar(stop_thresh_max_hue, stop_thresh_max_sat, stop_thresh_max_val);
    cv::inRange(hsv, low, high, thresh);

    const auto stop_line = get_line(thresh);

    #if PRINT_STOP_THRESH == 1
    debug::print_thresh(thresh);
    #endif

    const auto stop_line_rect = cv::boundingRect(stop_line);
    const cv::Rect2i detection_rect(cv::Point2i(0, expected_red_y - expected_red_radius), cv::Point2i(thresh.cols, expected_red_y + expected_red_radius));
    detected = stop_line_rect.area() >= stop_min_detect_area && rectangles_overlap(stop_line_rect, detection_rect);
}


void MicroCV::debug::print_thresh(cv::Mat1b& thresh) 
{
    auto size = thresh.size();

    printf("BEGIN THRESH\n");
    for (int i = 0; i < size.height; ++i) {
        for (int j = 0; j < size.width; ++j) {
            bool point = thresh(i,j) >> 7;
            printf("%x", point);
        }
        printf("\n");
    }
    printf("END THRESH \n");
}