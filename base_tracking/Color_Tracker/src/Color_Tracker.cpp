#include <iostream>

#include <opencv2/opencv.hpp>

#include "Particle_Filter.hpp"

//  TODO:
//  Do both system update and measurement update
//  Design and implement system update step
//      - Use estimated positions to calculate velocity and accel?
//      - Use flight data to update?
//  Improve histogram calculation step
//  Implement initial detection step
//  Comment and Document
//  Redesign weighing step?

int main (int argc, char *argv[ ])
{
    // Initial particle filter stuff
    std::vector<cv::Point2i> particles;
    std::vector<cv::Point2i> particles_update;
    std::vector<double> particle_weights;
    std::vector<cv::Point2i> estimated_pos;
    int num_iterations = 0;

    cv::Point2i initial_estimate;
    double initial_covariance = 100.0;
    double system_covariance = 100.0;
    double measurement_covariance = 100.0;

    int num_particles = 10;

    // Initialize vectors
    for(int i=0; i<num_particles; ++i)
    {
        int dx = 0 + (int)(sqrt(initial_covariance)*randn());
        int dy = 0 + (int)(sqrt(initial_covariance)*randn());

        particles.push_back(cv::Point2i((640/2)+dx, (480/2)+dy));
        particles_update.push_back(cv::Point2i((640/2)+dx, (480/2)+dy));
        particle_weights.push_back(0);
    }
    estimated_pos.push_back(cv::Point2i(640/2, 480/2));

    // Camera stuff
    cv::VideoCapture c(0);
    cv::Mat src, m, dst;

    cv::MatND initial_region_hist;
    cv::Mat initial_region;

    int key = 0;
    bool start = false;

    // Initial
    while(true)
    {
        // Get image
        c.read(m);
        src = m.clone();
        dst = m.clone();

        if(start == true)
        {
            // Make slice around center and calculate it's histogram
            if(num_iterations == 0)
            {
                initial_region = src(cv::Range(estimated_pos[num_iterations].y - 60,
                                                       estimated_pos[num_iterations].y + 60),
                                             cv::Range(estimated_pos[num_iterations].x - 60,
                                                       estimated_pos[num_iterations].x + 60)).clone();
                initial_region_hist = calc_hist(initial_region);
            }

            // Draw center slice
            draw_box(dst, cv::Point2i(estimated_pos[num_iterations].x+60, estimated_pos[num_iterations].y+60),
                          cv::Point2i(estimated_pos[num_iterations].x-60, estimated_pos[num_iterations].y-60),
                          cv::Scalar(0, 255, 0));



            /*
            // This step is very tentative!!!
            double gain = 0.1;
            if(num_iterations > 10) // Wait until we have enough information
            {
                for(int i=0; i<num_particles; ++i)
                {
                    // Update particles with system update and then add noise
                    int dx = (int)(gain * (estimated_pos[num_iterations].x - estimated_pos[num_iterations - 1].x));
                    int dy = (int)(gain * (estimated_pos[num_iterations].y - estimated_pos[num_iterations - 1].y));
                    dx += (int)(sqrt(measurement_covariance)*randn());
                    dy += (int)(sqrt(measurement_covariance)*randn());

                    particles[i].x = particles[i].x + dx;
                    particles[i].y = particles[i].y + dy;
                }
            }
            */

            // Measurement update and particle weighing step
            double sum_of_weights = 0.0;
            std::cout << "Iteration: " << num_iterations << std::endl;
            for(int i=0; i<num_particles; ++i)
            {
                // Update particles with system noise
                int dx = 0 + (int)(sqrt(measurement_covariance)*randn());
                int dy = 0 + (int)(sqrt(measurement_covariance)*randn());
                particles_update[i].x = particles[i].x + dx;
                particles_update[i].y = particles[i].y + dy;

                // Slice up image with updated particles
                cv::Mat roi = src(cv::Range(particles_update[i].y - 60,
                                            particles_update[i].y + 60),
                                  cv::Range(particles_update[i].x - 60,
                                            particles_update[i].x + 60)).clone();

                // Draw roi slices
                draw_box(dst, cv::Point2i(particles_update[i].x+60, particles_update[i].y+60),
                              cv::Point2i(particles_update[i].x-60, particles_update[i].y-60),
                              cv::Scalar(0, 0, 255));

                // Calculate particles weights based on roi histogram comparison
                cv::MatND hist = calc_hist(roi);
                double weight = calc_weight(initial_region_hist, hist);
                //double prob = exp(-20.0 * (weight*weight));

                //particle_weights[i] = prob;
                particle_weights[i] = weight;
                sum_of_weights += particle_weights[i];
            }

            // Normalize particle weights
            for(int i=0; i<num_particles; ++i)
            {
                particle_weights[i] /= sum_of_weights;
                std::cout << "Weight: " << particle_weights[i] << std::endl;
            }

            // Check resampling steps and redundancy of work
            // Resampling step 1
            double seq_sum = 0.0;
            std::vector<double> cumsum;
            for(int n=0; n<num_particles; ++n)
            {
                seq_sum += particle_weights[n];
                cumsum.push_back(seq_sum);
            }

            // Resampling step 2
            cv::Point2i result(0, 0);
            for(int i=0; i<num_particles; ++i)
            {
                for(int n=0; n<num_particles; ++n)
                {
                    double thresh = randu();
                    result = particles_update[n];
                    if(thresh <= cumsum[n])
                    {
                        break;
                    }
                }
                particles[i] = result;
                std::cout << particles[i] << std::endl;
            }

            // Take weighted avg of particles positions
            cv::Point2i sum;
            for(int i=0; i<num_particles; ++i)
            {
                sum.x += particles[i].x;
                sum.y += particles[i].y;
            }
            sum.x /= num_particles;
            sum.y /= num_particles;

            estimated_pos.push_back(sum);
            num_iterations++;

            // Plot estimated positions
            for(int i=0; i<estimated_pos.size(); ++i)
            {
                cv::circle(src, estimated_pos[i], 2, cv::Scalar(255, 255, 255), -1);
            }

            cv::imshow("dst", dst);
            cv::imshow("initial", initial_region);
        }
        key = cv::waitKey(5);
        if(key == 1048691)
        {
            start = true;
        }
        cv::imshow("src", src);
    }
}
