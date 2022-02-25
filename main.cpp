#include <opencv2/core.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <fstream>

#include "Regocnizer.hpp"

int main(int argc, char **argv)
{
    cv::String clKeys =
            "{mode     |<none>            | Keuze uit: interactive (i) of batch (b). }"
            "{file     |<none>            | Het bestand dat in batch-modus moet worden gebruikt. }"
            "{webcam   |<none>            | ID van de webcam. }";
    cv::CommandLineParser clParser(argc, argv, clKeys);

    //Do webcam shit anders standaard img
    cv::Mat originalImg;

    if(clParser.get<int>("webcam") >= 0)
    {
        cv::VideoCapture camera(0);

        if(!camera.isOpened())
        {
            std::cout << "error opening cam" << std::endl;
        }
        camera.read(originalImg);

    } else {
        std::string image_path = cv::samples::findFile("../img.png");
        originalImg = cv::imread(image_path, cv::IMREAD_COLOR);
    }

    Regocnizer regocnizer;

    if(clParser.get<std::string>("mode") == "i")
    {
        //regocnizer.createTrackbars(0);
        regocnizer.filterContinues(originalImg);

    } else if(clParser.get<std::string>("mode") == "b")
    {
        //regocnizer.createTrackbars(1);
        //parse batch
        std::string fileName = clParser.get<std::string>("file");
        std::ifstream batchFile(fileName);

        std::string temp;

        std::vector<std::string> shapes;
        std::vector<std::string> colours;

        while(std::getline(batchFile, temp))
        {
            temp.erase(std::find(temp.begin(), temp.end(), '#'), temp.end());
            if(temp.empty())
            {
                continue;
            }

            std::string shape = temp.substr(0, temp.find(" "));
            std::string colour = temp.substr(temp.find(" ") +1);

            shapes.push_back(shape);
            colours.push_back(colour);
        }
        regocnizer.runBatch(originalImg, shapes, colours);
    } else
    {
        std::cout << "Mee gegeven variabelen zijn incorrect" << std::endl;
    };



    int k = cv::waitKey(0); // Wait for a keystroke in the window
}