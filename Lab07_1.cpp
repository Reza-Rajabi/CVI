//
//  Lab07_1.cpp
//  Lab07
//
//

#include <opencv2/opencv.hpp>

const std::string windowName_0 = "Source Image";
const std::string windowName_1 = "Gray Image";
const std::string windowName_2 = "V_Edges Image";
const std::string windowName_3 = "H_Edges Image";
const std::string windowName_4 = "Magnitude Map";
const std::string windowName_5 = "Sobel Edge";
const std::string windowName_6 = "Laplace Edge";
const std::string windowName_7 = "Canny Edge";
const std::string windowName_8 = "Canny2 Edge";


int __main(int argc, char* argv[]) {
    
    // read the image from the project directory
    cv::Mat image = imread(argv[1], cv::IMREAD_COLOR);
    // Check if could find the image
    if( image.empty() ) {
        std::cout << "Could not read the image" << std::endl ;
        return 1;
    }
    
    // making a gray scaled image, save and display it
    cv::Mat image_gray;
    cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY, 1);
    cv::imwrite("gray.jpg", image_gray);
    
    // finding vertical edges with 3x3 kernel
    cv::Matx vKernel = cv::Matx<int,3,3>({-1,0,1,-2,0,2,-1,0,1});
    cv::Mat image_vEdge;
    filter2D(image_gray, image_vEdge, CV_8U, vKernel);
    cv::imwrite("vEdges.jpg", image_vEdge);
    
    // finding horizontal edges with 3x3 kernel
    cv::Matx hKernel = cv::Matx<int,3,3>({-1,-2,-1,0,0,0,1,2,1});
    cv::Mat image_hEdge;
    filter2D(image_gray, image_hEdge, CV_8U, hKernel);
    cv::imwrite("hEdges.jpg", image_hEdge);
    
    // magnitude map
    cv::Mat magnitudeMap = cv::Mat::ones(image_gray.rows, image_gray.cols, CV_8U);
    double Gh, Gv;
    for (int i = 0; i < image_gray.rows; i++) {
    for (int j = 0; j < image_gray.cols; j++) {
        Gh = image_hEdge.at<uchar>(i,j);
        Gv = image_vEdge.at<uchar>(i,j);
        magnitudeMap.at<uchar>(i,j) = sqrt(Gh*Gh + Gv*Gv);
    }}
    cv::imwrite("magMap.jpg", magnitudeMap);
    
    // sobel edge method
    cv::Mat sobel;
    cv::Sobel(image_gray, sobel, CV_8U, 1, 1);
    cv::imwrite("sobel.jpg", sobel);
    
    // laplacian edge method
    cv::Mat laplace;
    cv::Laplacian(image_gray, laplace, CV_8U, 1, 1);
    cv::imwrite("laplace.jpg", laplace);
    
    // canny edge method - 1, 2:1
    cv::Mat canny;
    cv::Canny(image_gray, canny, 125, 250);
    cv::imwrite("canny.jpg", canny);
    // canny edge method - 2, 3:1
    cv::Mat canny2;
    cv::Canny(image_gray, canny2, 100, 300);
    cv::imwrite("canny2.jpg", canny2);
    
    
    ///----------------------------------------------- DISPLAY ALL RESULTS ---------------------------------------------///
    // define named windows
    namedWindow(windowName_0, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_1, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_2, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_3, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_4, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_5, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_6, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_7, cv::WINDOW_AUTOSIZE);
    namedWindow(windowName_8, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName_1, 100, 100);
    cv::moveWindow(windowName_2, 200, 200);
    cv::moveWindow(windowName_3, 300, 300);
    cv::moveWindow(windowName_4, 400, 400);
    cv::moveWindow(windowName_5, 500, 500);
    cv::moveWindow(windowName_6, 600, 600);
    cv::moveWindow(windowName_7, 700, 700);
    cv::moveWindow(windowName_8, 800, 800);

    // while space-bar key (ASCII 32 ) has not pressed show the image
    while(cv::waitKey(33) != 32) {
        // display the images in the windows
        imshow(windowName_0, image);
        imshow(windowName_1, image_gray);
        imshow(windowName_2, image_vEdge);
        imshow(windowName_3, image_hEdge);
        imshow(windowName_4, magnitudeMap);
        imshow(windowName_5, sobel);
        imshow(windowName_6, laplace);
        imshow(windowName_7, canny);
        imshow(windowName_8, canny2);
    }
    
    //cv::destroyAllWindows();
    return 0;
}
