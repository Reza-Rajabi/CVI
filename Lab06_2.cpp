//
//  Lab06_2.cpp
//  Lab06
//
//

#include <opencv2/opencv.hpp>

const std::string imageName = "unnamed.jpg";
const std::string windowName_0 = "Source Image";
const std::string windowName_1 = "Processed Image";

double getUserEntry (std::string message, int min, int max);

int main( /** No argument is needed */ ) {
    
    // read the image from the project directory
    cv::Mat image = imread(imageName, cv::IMREAD_COLOR);
    // Check if could find the image
    if( image.empty() ) {
        std::cout << "Could not read the image" << std::endl ;
        return 1;
    }
     
    std::string menu = "0 Exit\n1 Rotate\n2 Resize\n3 Perspective\n";

    cv::namedWindow(windowName_0);
    cv::namedWindow(windowName_1);
    cv::moveWindow(windowName_1, 200, 100);
    
    cv::imshow(windowName_0, image);
    cv::waitKey(33);
    
    cv::Mat processed_image;
    int choice = (int) getUserEntry("Enter a choice\n" + menu, 0, 3);
    
    while (choice != 0) {
    
        if (choice == 1) {
            double angle = getUserEntry("Enter an angle to rotate.", -360, 360);
            
            cv::Point2f centre = cv::Point2f(image.cols/2.0, image.rows/2.0);
            cv::Mat rMat = cv::getRotationMatrix2D(centre, angle, 1.0);
            cv::warpAffine(image, processed_image, rMat, image.size());
            cv::imshow(windowName_1, processed_image);
            cv::waitKey(33);
        }
        else if (choice == 2) {
            double fX = getUserEntry("Enter resizing factor for X: ", 0, INT_MAX);
            double fY = getUserEntry("Enter resizing factor for Y: ", 0, INT_MAX);
            
            cv::resize(image, processed_image, cv::Size(), fX, fY);
            cv::imshow(windowName_1, processed_image);
            cv::waitKey(33);
        }
        else if (choice == 3) {
            cv::Matx<double,3,3> homographyMat = {0.4,-0.4,190, 0.15,0.4,100, 0,0,1};
            cv::warpPerspective(image, processed_image, homographyMat, image.size());
            cv::imshow(windowName_1, processed_image);
            cv::waitKey(33);
        }
    
        choice = (int) getUserEntry("\nEnter a choice\n" + menu, 0, 3);

    }
    
    cv::destroyAllWindows();
    return 0;
    
}

double getUserEntry (std::string message, int min, int max) {
    double* answer = nullptr;
    std::cout << message << std::endl;
    while ( answer == nullptr ) {
        answer = new double;
        std::cout << "Your entry: ";
        std::cin >> *answer;
        if (!std::cin.fail() && *answer >= min && *answer <= max) /** do nothing */;
        else {
            std::cin.clear();
            std::cout << "Please try again.\n" << std::endl;
            delete answer;
            answer = nullptr;
        }
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
    }
    return *answer;
}
