//
//  Lab02.cpp
//  Lab02
//
//  Example 2-10. The same object can load videos from a camera or a file
//  Partialy taken from:
//      https://github.com/oreillymedia/Learning-OpenCV-3_examples/blob/master/example_02-10.cpp
//

#include <opencv2/opencv.hpp>
#include <iostream>

void help(char** argv ) {
    std::cout << "\n"
    << "\nxample 2-10. The same object can load videos from a camera or a file"
    << "\nCall:\n"
    << argv[0] <<" [path/image]\n"
    << "\nor, read from camera:\n"
    << argv[0]
    << "\nFor example:\n"
    << argv[0] << " ../tree.avi\n"
    << std::endl;
}


int main( int argc, char** argv ) {
    
    help(argv);
    
    cv::namedWindow( "Example 2-10", cv::WINDOW_AUTOSIZE );
    cv::VideoCapture cap;
    
    if (argc==1) {
        cap.open(0); // open the first camera
    } else {
        cap.open(argv[1]);
    }
    
    if( !cap.isOpened() ) { // check if we succeeded
        std::cerr << "Couldn't open capture." << std::endl;
        return -1;
    }
    
    /*
     * Lab02 - Part1 - Subheading 5
     *
     * a) capture-object properties
     */
    std::cout << "The width of the capture object: ";
    std::cout << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "The height of the capture object: ";
    std::cout << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    
    /*
    * Lab02 - Part1 - Subheading 5
    *
    * b) press x to save a snapshot
    *    snapshot counter for image naming
    */
    int counter = 0;
    
    cv::Mat frame;
    
    for(;;) {
        
        cap >> frame;
        
        if( frame.empty() ) break; // Ran out of film
        
        cv::imshow( "Example 2-10", frame );
        
        /*
        * Lab02 - Part1 - Subtitle 5
        *
        * b) press x to save a snapshot (ASCII 120)
        */
        int keyEntered = cv::waitKey(33);
        if ( keyEntered == 120 ) {
            // freeze the frame for 1 second
            cv::waitKey(1000);
            // can have maximum 99 snapshots otherwise override image
            counter = ++counter > 99 ? 0 : counter;
            // store a snapshot
            std::string imageName = std::string("image") + (counter < 10 ? "0" : "") \
                                    + std::to_string(counter) + std::string(".png");
            bool couldSaveImage = cv::imwrite(imageName, frame);
            std::cout << ( couldSaveImage ? "Stored a snapshot." : "Could not store the snapshot.") << std::endl;
        }
        
        /*
        * Lab02 - Part1 - Subtitle 5
        *
        * c) press Space bar or Enter key to exit (ASCII 32 or 13)
        */
        else if ( keyEntered == 32 || keyEntered == 13) break;
        
        /*
        * Lab02 - Part1 - Subtitle 5
        *
        * d) ignore all other keys entered
        */
        else { /*do nothing*/ }
        
    }
    
    return 0;
    
}
