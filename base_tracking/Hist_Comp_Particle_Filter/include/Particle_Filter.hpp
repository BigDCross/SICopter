#ifndef PARTICLE_FILTER
#define PARTICLE_FILTER

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <random>

#define PI 3.14159265

std::default_random_engine rand_gen;
std::normal_distribution<double> norm_dist(0.0, 1.0); // gaussian around 0 with std dev 1
std::uniform_real_distribution<double> uniform_dist(0.0, 1.0); // uniform from 0-1

double randn()
{
    return norm_dist(rand_gen);
}

double randu()
{
    return uniform_dist(rand_gen);
}

double mean(std::vector<double> arr)
{
    double sum = 0.0;
    for(int i=0; i<arr.size(); i++)
    {
        sum += arr[i];
    }
    return sum / (double)arr.size();
}

cv::MatND calc_hist (const cv::Mat roi)
{
    // TODO: Figure out best hist params
    cv::MatND hist;

    // Calc initial histogram
    int channels[] = {0, 1, 2};
    int histSize[] = {50, 50, 50};
    //int channels[] = {2};
    //int histSize[] = {50};
    float range[] = {0, 256};
    const float* ranges[] = {range, range, range};
    //const float* ranges[] = {range};
    cv::calcHist(&roi, 1, channels, cv::Mat(), // do not use mask
            hist, 3, histSize, ranges,
            true, // the histogram is uniform
            false );

    return hist;
}

double calc_weight(cv::MatND &initial_hist, cv::MatND &current_hist)
{
    double weight = compareHist(current_hist, initial_hist, CV_COMP_CORREL);
    return weight;
}

int draw_box(cv::Mat &src, cv::Point2i x, cv::Point2i y, cv::Scalar color)
{
    cv::rectangle(src, x, y, color, 2);
}

#endif
