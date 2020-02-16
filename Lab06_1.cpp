//
//  Lab06_1.cpp
//  Lab06
//
//

#include <opencv2/opencv.hpp>

const std::string imageName = "unnamed.jpg";
const std::string windowName_0 = "Source Image";
const std::string windowName_1 = "Gray Image";
const std::string windowName_2 = "Binary Image";
const std::string windowName_3 = "Morphed Image open";
const std::string windowName_4 = "Morphed Image close";
const std::string windowName_5 = "Morphed Image open 7x7";
const std::string windowName_6 = "Morphed Image close 7x7";



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
    cv::imwrite("gray.jpg", image_gray);
    
    // Apply threshold 128 to the gray image
    cv::Mat image_binary;
    cv::threshold(image_gray, image_binary, 128, 255, cv::THRESH_BINARY);
    cv::imwrite("binary.jpg", image_binary);
    
    // Applying morphology- 3x3 kernel open
    cv::Mat image_morphed;
    cv::Mat kernel = 255 * cv::Mat::ones(3, 3, CV_8UC1);
    cv::morphologyEx(image_binary, image_morphed, cv::MORPH_OPEN, kernel);
    cv::imwrite("morphed_3x3_o.jpg", image_morphed);
    
    // Applying morphology- 3x3 kernel close
    cv::Mat image_morphed2;
    cv::morphologyEx(image_binary, image_morphed2, cv::MORPH_CLOSE, kernel);
    cv::imwrite("morphed_3x3_c.jpg", image_morphed2);
    
    // Applying morphology- 7x7 kernel open
    cv::Mat image_morphed3;
    cv::Mat kernel_ = 255 * cv::Mat::ones(7, 7, CV_8UC1);
    cv::morphologyEx(image_binary, image_morphed3, cv::MORPH_OPEN, kernel_);
    cv::imwrite("morphed_7x7_o.jpg", image_morphed3);
    
    // Applying morphology- 7x7 kernel close
    cv::Mat image_morphed4;
    cv::morphologyEx(image_binary, image_morphed4, cv::MORPH_CLOSE, kernel_);
    cv::imwrite("morphed_7x7_c.jpg", image_morphed4);
    
    
    // define named windows
    namedWindow(windowName_0, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_1, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_2, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_3, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_4, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_5, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_6, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName_1, 100, 100);
    cv::moveWindow(windowName_2, 250, 100);
    cv::moveWindow(windowName_3, 400, 100);

    
    // while space-bar key (ASCII 32 ) has not pressed show the image
    while(cv::waitKey(33) != 32) {
        // display the images in the windows
        imshow(windowName_0, image);
        imshow(windowName_1, image_gray);
        imshow(windowName_2, image_binary);
        imshow(windowName_3, image_morphed);
        imshow(windowName_4, image_morphed2);
        imshow(windowName_5, image_morphed3);
        imshow(windowName_6, image_morphed4);
    }
    
    cv::destroyAllWindows();
    return 0;
    
}
