//
//  Lab05.cpp
//  Lab05
//
//

#include <opencv2/opencv.hpp>
#include <iostream>


const std::string imageName = "unnamed.jpg";
const std::string windowName_0 = "Source Image";
const std::string windowName_1 = "Gray Image";
const std::string windowName_2 = "Equall Hist";

int __main( /** No argument is needed */ ) {
    
    // read the image from the project directory
    cv::Mat image = imread(imageName, cv::IMREAD_COLOR);
    // Check if could find the image
    if( image.empty() ) {
        std::cout << "Could not read the image" << std::endl ;
        return 1;
    }
     
    // making a gray scaled image from the first image
    cv::Mat image_gray;
    cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY, 1);
    
    // Apply histogram equalization to the gray image
    cv::Mat image_equall_hist;
    cv::equalizeHist(image_gray, image_equall_hist);
    
    // define named windows
    namedWindow(windowName_0, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_1, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_2, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName_1, 200, 100);
    cv::moveWindow(windowName_2, 400, 100);
    
    // while space-bar key (ASCII 32 ) has not pressed show the image
    while(cv::waitKey(33) != 32) {
        // display the images in the windows
        imshow(windowName_0, image);
        imshow(windowName_1, image_gray);
        imshow(windowName_2, image_equall_hist);
    }
    
    cv::destroyAllWindows();
    return 0;
    
}
