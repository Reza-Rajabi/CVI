//
//  Lab07_2.cpp
//  Lab07
//
//

#include <opencv2/opencv.hpp>

const std::string windowName_0 = "Source Image";
const std::string windowName_1 = "Canny Image";
const std::string windowName_2 = "Hough Image";


int main() {
    
    // read the image from the project directory
    cv::Mat image = imread("Yonge_Street.jpg", cv::IMREAD_GRAYSCALE);
    // Check if could find the image
    if( image.empty() ) {
        std::cout << "Could not read the image" << std::endl ;
        return 1;
    }
    
    // canny edge method, 3:1 ratio
    cv::Mat canny;
    cv::Canny(image, canny, 100, 300);
    cv::imwrite("canny.jpg", canny);
    
    // Hough transform line detection
    cv::Mat imageWithLines;
    cv::cvtColor(canny, imageWithLines, cv::COLOR_GRAY2BGR);
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(canny, lines, 1, CV_PI / 180, 200, 0, 0);
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        cv::line(imageWithLines, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        std::cout << "Line " << i << ": (rho: " << rho << ", theta: " << theta << ")" << std::endl;
    }
    cv::imwrite("houghLines.jpg", imageWithLines);
    
    // Hough transform circle detection
    cv::Mat imageWithCircles;
    imageWithLines.copyTo(imageWithCircles);
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(canny, circles, cv::HOUGH_GRADIENT, 10, 100);
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point centre = cv::Point(circles[i][0], circles[i][1]);
        cv::circle(imageWithCircles, centre, circles[i][2], cv::Scalar(0,255,0));
    }
    cv::imwrite("houghBoth.jpg", imageWithCircles);
    
    
    ///----------------------------------------------- DISPLAY ALL RESULTS ---------------------------------------------///
    // define named windows
    namedWindow(windowName_0, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_1, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_2, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName_1, 300, 300);
    cv::moveWindow(windowName_2, 600, 600);

    // while space-bar key (ASCII 32 ) has not pressed show the image
    while(cv::waitKey(33) != 32) {
        // display the images in the windows
        imshow(windowName_0, image);
        imshow(windowName_1, canny);
        imshow(windowName_2, imageWithCircles);
    }
    
    return 0;
    
}
