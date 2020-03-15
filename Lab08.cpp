//
//  Lab08.cpp
//  Lab08
//
//

#include <opencv2/opencv.hpp>

const char predicts[] = "alg_bin.csv";
const char continuousPredicts[] = "alg_dbl.csv";
const char lables[] = "gt.csv";
const char imgPredictsFile1[] = "bitmap_A1.png";
const char imgPredictsFile2[] = "bitmap_A2.png";
const char imgLablesFile[] = "bitmap_gt.png";
const char generalMessage[] = "Select a number (1 to 4) or 0 to exit: ";

int getUserEntry (std::string message);
void openStream(const char* file, std::ifstream& ifs);
int countRows(std::ifstream& ifs);
template <typename T> void loadData(std::ifstream& ifs, T* data, int len);
void loadImage(cv::Mat& img, const char* imgName);
int calcPR(bool* pred, bool* gt, int len, double* P, double* R);
double calcFb(double P, double R, double beta);
void thresh_v(double* A, bool* B, int len, double thresh);
void thresh_img(cv::Mat img, bool* B, double thresh);



int main() {
    std::ofstream ofs;
    std::ifstream ifs;
    
    /// variables for part 1 and 2
    openStream(predicts, ifs);
    int len = countRows(ifs);
    bool pred[len], gt[len];
    double A[len];
    bool B[len];
    double P, R, F;
    int status;
    
    openStream(predicts, ifs);
    loadData<bool>(ifs, pred, len);
    openStream(lables, ifs);
    loadData<bool>(ifs, gt, len);
    openStream(continuousPredicts, ifs);
    loadData<double>(ifs, A, len);
    
    /// variables for part 3 and 4
    cv::Mat imgPredicts1, imgPredicts2, imgLables;
    loadImage(imgPredicts1, imgPredictsFile1);
    loadImage(imgPredicts2, imgPredictsFile2);
    loadImage(imgLables, imgLablesFile);
    
    int len_img = imgLables.cols * imgLables.rows;
    bool B_predicts[len_img];
    bool B_lables[len_img];

    
    int choice = getUserEntry(generalMessage);
    while (choice != 0) {
        if (choice == 1) {
            status = calcPR(pred, gt, len, &P, &R);
            if (status == 0) { std::cout << "Error calling calcPR \n"; exit(1); }
            F = calcFb(P, R, 1.0);
            
            std::cout << "Precision: " << P << std::endl;
            std::cout << "Recall: " << R << std::endl;
            std::cout << "F1: " << F << std::endl << std::endl;
        }
        else if (choice == 2) {
            ofs.open("PR.csv");
            if (ofs.is_open()) {
                ofs << "THRESH,P,R,F1" << std::endl;
                ofs << std::setprecision(3);
            }
            for(double thresh = 0.1; thresh < 1.0; thresh += 0.1) {
                thresh_v(A, B, len, thresh);
                status = calcPR(B, gt, len, &P, &R);
                if (status == 0) { std::cout << "Error calling calcPR \n"; exit(1); }
                F = calcFb(P, R, 1.0);
                
                ofs << thresh << "," << P << "," << R << "," << F;
                ofs << std::endl;
            }
            ofs.close();
            std::cout << "Done!" << std::endl << std::endl;
        }
        else if (choice == 3) {
            thresh_img(imgPredicts1, B_predicts, 128);
            thresh_img(imgLables, B_lables, 128);

            status = calcPR(B_predicts, B_lables, len_img, &P, &R);
            if (status == 0) { std::cout << "Error calling calcPR \n"; exit(1); }
            F = calcFb(P, R, 1.0);
            
            std::cout << "Precision: " << P << " Recall: " << R << " F1: " << F;
            std::cout << std::endl << std::endl;
        }
        else if (choice == 4) {
            ofs.open("PR_img.csv");
            if (ofs.is_open()) {
                ofs << "THRESH,P,R,F1" << std::endl;
                ofs << std::setprecision(4);
            }
            for(int thresh = 25; thresh <= 250; thresh += 25) {
                thresh_img(imgPredicts2, B_predicts, thresh);
                thresh_img(imgLables, B_lables, thresh);
                status = calcPR(B_predicts, B_lables, len_img, &P, &R);
                if (status == 0) { std::cout << "Error calling calcPR \n"; exit(1); }
                F = calcFb(P, R, 1.0);
                
                ofs << thresh << "," << P << "," << R << "," << F;
                ofs << std::endl;
            }
            ofs.close();
            std::cout << "Done!" << std::endl << std::endl;
        }
        
        choice = getUserEntry(generalMessage);
    }
    
    std::cout << std::endl;
    return 0;
    
}

int getUserEntry (std::string message) {
    int answer = -1;
    while ( answer == -1 ) {
        std::cout << message;
        std::cin >> answer;
        if (!std::cin.fail() && answer >= 1 && answer <= 4) /** do nothing */;
        else if ( answer == 0 ) /** do nothing */;
        else {
            std::cin.clear();
            std::cout << "Please try again.\n" << std::endl;
            answer = -1;
        }
        /** flush the buffer */
        std::cin.ignore(INT_MAX, '\n');
    }
    return answer;
}

void openStream(const char* file, std::ifstream& in) {
    in.open(file, std::ifstream::in);
    if (in.fail()) {
        std::cout << "Couldn't open data file." << std::endl;
        exit(1);
    }
}

int countRows(std::ifstream& in) {
    int counter = 0;
    std::string dispose;
    while(getline(in, dispose)) ++counter;
    in.close();
    return counter;
}

template <typename T>
void loadData(std::ifstream& in, T* data, int len) {
    for (int i = 0; i < len; i++) {
        in >> data[i];
        in.ignore(); /// discard the \n character
    }
    in.clear();
    in.close();
}

void loadImage(cv::Mat& img, const char* imgName) {
    img = cv::imread(imgName, cv::IMREAD_GRAYSCALE);
    if( img.empty() ) {
        std::cout << "Could not read the image" << std::endl ;
        exit(1);
    }
}

int calcPR(bool* pred, bool* gt, int len, double* P, double* R) {
    int succeed = 1;
    double TP = 0, TN = 0, FP = 0, FN = 0;
    if (pred && gt && P && R) {     /// if all pointers are valid
        for (int i = 0; i < len; i++) {
            if (pred[i] && gt[i]) TP++;
            else if (pred[i] && !gt[i]) FP++;
            else if (!pred[i] && gt[i]) FN++;
            else if (!pred[i] && !gt[i]) TN++;
        }
        *R = (TP + FN == 0) ? 1.0 : TP/(TP + FN);
        *P = (TP + FP == 0) ? 1.0 : TP/(TP + FP);
    }
    else succeed = 0;
    
    return succeed;
}

double calcFb(double P, double R, double beta) {
    double b = pow(beta,2);
    return (1 + b) * P * R / (b * P + R);
}

void thresh_v(double* A, bool* B, int len, double thresh) {
    for (int i = 0; i < len; i++) {
        B[i] = A[i] > thresh;
    }
}

void thresh_img(cv::Mat img, bool* B, double thresh) {
    int cols = img.cols;
    for(int r = 0; r < img.rows; r++) {
        for(int c = 0; c < img.cols; c++) {
            B[r*cols + c] = img.at<uchar>(r,c) > thresh;
        }
    }
}
