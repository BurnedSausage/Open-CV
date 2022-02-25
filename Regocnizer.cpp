#include "Regocnizer.hpp"


void Regocnizer::setLabel(cv::Mat &im, const std::string label, std::vector<cv::Point>& contour, bool batch, clock_t time)
{
    unsigned long int timeTaken = clock() - time;

    int fontType = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontType, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);
    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2 - 20));
    cv::putText(im, label, pt, fontType, scale, CV_RGB(0,0,0), thickness, 8);

    cv::Point pt2(r.x + (r.width / 2), r.y + (r.height / 2));
    cv::circle(im, pt2, 8, (255, 255, 255), -1);
    cv::Point pt3(r.x + (r.width / 2) + 8, r.y + (r.height / 2));
    cv::putText(im, std::to_string(r.x + (r.width / 2)) + " " + std::to_string(r.y + (r.height / 2)), pt3, fontType, scale, CV_RGB(0,0,0), thickness, 8);

    text = cv::getTextSize(std::to_string(cv::contourArea(contour)), fontType, scale, thickness, &baseline);
    cv::Point pt4(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2 + 20));
    cv::putText(im, std::to_string(cv::contourArea(contour)), pt4, fontType, scale, CV_RGB(0,0,0), thickness, 8);

    text = cv::getTextSize(std::to_string(timeTaken), fontType, scale, thickness, &baseline);
    cv::Point pt5(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2 + 30));
    cv::putText(im, std::to_string(timeTaken), pt5, fontType, scale, CV_RGB(0,0,0), thickness, 8);

    if(batch)
    {
        //print this info
        std::cout << label << std::endl;
        std::cout << "Middle point is: " << pt2 << std::endl;
        std::cout << "Area is: " << std::to_string(cv::contourArea(contour)) << std::endl;
        std::cout << "Time taken is: " << timeTaken << std::endl;
        std::cout << std::endl;
    }
}

cv::Mat Regocnizer::filterColour(cv::Mat input, cv::Scalar lower, cv::Scalar upper)
{
    cv::Mat hsvImg;
    cv::cvtColor(input, hsvImg, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsvImg, lower, upper, mask);

    cv::Mat afterMask;
    cv::bitwise_and(input, input, afterMask, mask);

    return afterMask;
}

cv::Mat Regocnizer::shapeDetection(cv::Mat originalImg, cv::Mat colourImg, std::string colour, clock_t time)
{
    //Gray scale image

    cv::Mat grayImg;
    cv::cvtColor(colourImg, grayImg, cv::COLOR_BGR2GRAY);

    //Grab borders of shape

    cv::Mat borderImg;
    cv::Canny(grayImg, borderImg, 100, 200);

    //List all the contours and draw them

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(borderImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::drawContours(originalImg, contours, -1, (0,255,0), 3);

    //Shape detection

    std::vector<cv::Point> approx;

    for(long unsigned int i = 0; i < contours.size(); ++i)
    {
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

        if(approx.size() == 3)
        {
            setLabel(originalImg, colour + " Driehoek", contours[i], false, time);
        }else if(approx.size() == 4){
            setLabel(originalImg, colour + " Rechthoek", contours[i], false, time);
        } else
        {
            setLabel(originalImg, colour + " Halve Cirkel", contours[i], false, time);
        }

    }

    return originalImg;
}

cv::Mat Regocnizer::shapeDetection(cv::Mat originalImg, cv::Mat colourImg, std::string colour, int shape, clock_t time)
{
    //Gray scale image
    cv::Mat grayImg;
    cv::cvtColor(colourImg, grayImg, cv::COLOR_BGR2GRAY);

    //Grab borders of shape
    cv::Mat borderImg;
    cv::Canny(grayImg, borderImg, 100, 200);

    //List all the contours and draw them
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(borderImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::drawContours(originalImg, contours, -1, (0,255,0), 3);

    //Shape detection
    std::vector<cv::Point> approx;

    for(long unsigned int i = 0; i < contours.size(); ++i)
    {
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

        if(approx.size() == 3 && shape == 3)
        {
            setLabel(originalImg, colour + " Driehoek", contours[i], true, time);
        }else if(approx.size() == 4 && shape == 4){
            setLabel(originalImg, colour + " Rechthoek", contours[i], true, time);
        } else if(shape == 5 && approx.size() != 3 && approx.size() != 4)
        {
            setLabel(originalImg, colour + " Halve Cirkel", contours[i], true, time);
        }

    }

    return originalImg;
}

void Regocnizer::filterContinues(cv::Mat originalImg) {
    ogImg = originalImg;
    clock_t time = clock();
    std::cout << orangeLower << std::endl;
    cv::Mat orangeImg = filterColour(originalImg, orangeLower, orangeUpper);
    cv::Mat greenImg = filterColour(originalImg, greenLower, greenUpper);
    cv::Mat pinkImg = filterColour(originalImg, pinkLower, pinkUpper);
    cv::Mat yellowImg = filterColour(originalImg, yellowLower, yellowUpper);

    originalImg = shapeDetection(originalImg, orangeImg, "Oranje", time);
    originalImg = shapeDetection(originalImg, greenImg, "Groen", time);
    originalImg = shapeDetection(originalImg, pinkImg, "Roze", time);
    originalImg = shapeDetection(originalImg, yellowImg, "Geel", time);

    cv::imshow("Output", originalImg);
    cv::imshow("Mask", orangeImg);
}

void Regocnizer::runBatch(cv::Mat originalImg, std::vector<std::string> shape, std::vector<std::string> colour)
{
    ogImg = originalImg;
    shaps = shape;
    colors = colour;

    clock_t time = clock();
    for(long unsigned int i = 0; i < shape.size(); i++)
    {
        //start timer
        cv::Scalar upper;
        cv::Scalar lower;
        if(colour[i] == "oranje")
        {
            upper = orangeUpper;
            lower = orangeLower;
        } else if(colour[i] == "groen")
        {
            upper = greenUpper;
            lower = greenLower;
        } else if(colour[i] == "roze")
        {
            upper = pinkUpper;
            lower = pinkLower;
        } else if(colour[i] == "geel")
        {
            upper = yellowUpper;
            lower = yellowLower;
        } else {
            std::cout << "incorrecte kleur in batch file" << std::endl;
        }
        cv::Mat filteredImg = filterColour(originalImg, lower, upper);

        int outline = 0;
        if(shape[i] == "driehoek")
        {
            outline = 3;
        } else if(shape[i] == "rechthoek")
        {
            outline = 4;
        } else if(shape[i] == "halveCirkel")
        {
            outline = 5;
        } else {
            std::cout << "incorrecte vorm in batch file" << std::endl;
        }

        originalImg = shapeDetection(originalImg, filteredImg, colour[i], outline, time);
    }
    cv::imshow("Output", originalImg);
}

void Regocnizer::setBoundries(int, void* object)
{
    Regocnizer* reg = reinterpret_cast<Regocnizer*>(object);

    reg->orangeLower = cv::Scalar(cv::getTrackbarPos("OrangeLowerH", "windowOrange"), cv::getTrackbarPos("OrangeLowerS", "windowOrange"), cv::getTrackbarPos("OrangeLowerV", "windowOrange"));
    reg->orangeUpper = cv::Scalar(cv::getTrackbarPos("OrangeUpperH", "windowOrange"), cv::getTrackbarPos("OrangeUpperS", "windowOrange"), cv::getTrackbarPos("OrangeUpperV", "windowOrange"));

    reg->greenLower = cv::Scalar(cv::getTrackbarPos("GreenLowerH", "windowGreen"), cv::getTrackbarPos("GreenLowerS", "windowGreen"), cv::getTrackbarPos("GreenLowerV", "windowGreen"));
    reg->greenUpper = cv::Scalar(cv::getTrackbarPos("GreenUpperH", "windowGreen"), cv::getTrackbarPos("GreenUpperS", "windowGreen"), cv::getTrackbarPos("GreenUpperV", "windowGreen"));

    reg->pinkLower = cv::Scalar(cv::getTrackbarPos("PinkLowerH", "windowPink"), cv::getTrackbarPos("PinkLowerS", "windowPink"), cv::getTrackbarPos("PinkLowerV", "windowPink"));
    reg->pinkUpper = cv::Scalar(cv::getTrackbarPos("PinkUpperH", "windowPink"), cv::getTrackbarPos("PinkUpperS", "windowPink"), cv::getTrackbarPos("PinkUpperV", "windowPink"));

    reg->yellowLower = cv::Scalar(cv::getTrackbarPos("YellowLowerH", "windowYellow"), cv::getTrackbarPos("YellowLowerS", "windowYellow"), cv::getTrackbarPos("YellowLowerV", "windowYellow"));
    reg->yellowUpper = cv::Scalar(cv::getTrackbarPos("YellowUpperH", "windowYellow"), cv::getTrackbarPos("YellowUpperS", "windowYellow"), cv::getTrackbarPos("YellowUpperV", "windowYellow"));

    if(reg->batch == 0)
    {
        reg->filterContinues(reg->ogImg);
        std::cout << "done" << std::endl;
    } else {
        reg->runBatch(reg->ogImg, reg->shaps, reg->colors);
    }
}

void Regocnizer::createTrackbars(int btch)
{
    batch = btch;
    cv::namedWindow("windowOrange", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("OrangeUpperH", "windowOrange", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("OrangeLowerH", "windowOrange", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("OrangeUpperS", "windowOrange", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("OrangeLowerS", "windowOrange", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("OrangeUpperV", "windowOrange", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("OrangeLowerV", "windowOrange", 0, 255, &Regocnizer::setBoundries, this);

    cv::namedWindow("windowGreen", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("GreenUpperH", "windowGreen", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("GreenLowerH", "windowGreen", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("GreenUpperS", "windowGreen", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("GreenLowerS", "windowGreen", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("GreenUpperV", "windowGreen", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("GreenLowerV", "windowGreen", 0, 255, &Regocnizer::setBoundries, this);

    cv::namedWindow("windowPink", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("PinkUpperH", "windowPink", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("PinkLowerH", "windowPink", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("PinkUpperS", "windowPink", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("PinkLowerS", "windowPink", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("PinkUpperV", "windowPink", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("PinkLowerV", "windowPink", 0, 255, &Regocnizer::setBoundries, this);

    cv::namedWindow("windowYellow", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("YellowUpperH", "windowYellow", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("YellowLowerH", "windowYellow", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("YellowUpperS", "windowYellow", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("YellowLowerS", "windowYellow", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("YellowUpperV", "windowYellow", 0, 255, &Regocnizer::setBoundries, this);
    cv::createTrackbar("YellowLowerV", "windowYellow", 0, 255, &Regocnizer::setBoundries, this);
}