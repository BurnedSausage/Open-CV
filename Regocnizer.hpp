#ifndef BEELDHERKENNINH_REGOCNIZER_HPP
#define BEELDHERKENNINH_REGOCNIZER_HPP

#include <opencv2/core.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include <time.h>

class Regocnizer {
public:
    /**
     * constructor
     */
    Regocnizer(){};

    /**
     * Constructor
     * @param ogImage construct with the image to use
     */
    Regocnizer(cv::Mat ogImage){ogImg = ogImage};
    /**
     * destructor
     */
    ~Regocnizer(){};

    /**
     *The fuction to filter all colours and shapes at once
     * @param originalImg the cv::Mat to filter out of.
     */
    void filterContinues(cv::Mat originalImg);

    void runBatch(cv::Mat originalImg, std::vector<std::string> shape, std::vector<std::string> colour);

    /**
     * The function to call settings window
     * @param batch 1 if its batch processing needed for trackbar callback func
     */
    //void createTrackbars(int batch);

private:
    /**
 * Label function to create all tekst on shapes
 * @param im The image text should be added to
 * @param label The text of the label
 * @param contour The shape the text should be added to
 * @param batch If batch == true then some info will be printed in terminal
 */
    void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour, bool batch, clock_t time);

    /**
     * Function to filter out a colour
     * @param input Input image to filter out of
     * @param lower Lower hsv range
     * @param upper Upper hsv range
     * @return Image with just the wanted colour cv::Mat
     */
    cv::Mat filterColour(cv::Mat input, cv::Scalar lower, cv::Scalar upper);

    /**
     * Shapedetection funtion
     * @param originalImg The image to detect shapes in
     * @param colourImg The image where the colour is seperated
     * @param colour The name of the colour to be added to the label
     * @return Matrix img with the shapes drawn on them and labels.
     */
    cv::Mat shapeDetection(cv::Mat originalImg, cv::Mat colourImg, std::string colour, clock_t time);

    /**
     * Shapedetection funtion
     * @param originalImg The image to detect shapes in
     * @param colourImg The image where the colour is seperated
     * @param colour The name of the colour to be added to the label
     * @param shape The name of the shape to detect.
     * @return Matrix img with the shapes drawn on them and labels.
     */
    cv::Mat shapeDetection(cv::Mat originalImg, cv::Mat colourImg, std::string colour, int contours, clock_t time);

    static void setBoundries(int x, void *object);

    /**
     * Upper and lower ranges of the colours filtered.
     */
    cv::Scalar orangeLower = cv::Scalar(10,100,160);
    cv::Scalar orangeUpper = cv::Scalar(25,255,255);
    cv::Scalar greenLower = cv::Scalar(35,50,50);
    cv::Scalar greenUpper = cv::Scalar(70,255,255);
    cv::Scalar yellowLower = cv::Scalar(25,110,160);
    cv::Scalar yellowUpper = cv::Scalar(35,255,255);
    cv::Scalar pinkLower = cv::Scalar(120,120,180);
    cv::Scalar pinkUpper = cv::Scalar(180,255,255);

//    int batch = 0;
//    cv::Mat ogImg;
//    std::vector<std::string> shaps;
//    std::vector<std::string> colors;

};


#endif //BEELDHERKENNINH_REGOCNIZER_HPP
