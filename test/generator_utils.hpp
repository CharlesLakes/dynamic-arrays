#ifndef GENERATOR_UTILS_HPP
#define GENERATOR_UTILS_HPP

#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>

// Function to get the current date and time as a string
std::string get_current_date() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now); // Renamed to avoid conflict
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_now), "%Y%m%d_%H%M%S");
    return ss.str();
}

/**
 * @brief A class for generating random numbers and managing seeds.
 */
class RandomGenerator {
private:
    unsigned int seed_;        // The seed used for random number generation.
    std::mt19937 gen_;         // Mersenne Twister random number engine.
    bool seed_from_args_;      // Flag indicating if the seed was provided via command line.

public:
    /**
     * @brief Constructs a RandomGenerator object.
     *
     * Parses command line arguments for a "--seed" option. If a seed is provided
     * and is valid, it's used. Otherwise, a seed is generated using std::random_device.
     * Prints a message indicating the seed source and value.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line arguments.
     */
    RandomGenerator(int argc, char* argv[]) : seed_from_args_(false) {
        // Parse command line arguments for --seed
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--seed" && i + 1 < argc) {
                try {
                    seed_ = std::stoul(argv[i + 1]);
                    seed_from_args_ = true;
                    std::cout << "Using seed from command line: " << seed_ << std::endl;
                    break;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid seed value '" << argv[i + 1] << "' (invalid_argument). Using random seed." << std::endl;
                    seed_from_args_ = false; // Fallback to random_device
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: Invalid seed value '" << argv[i + 1] << "' (out_of_range). Using random seed." << std::endl;
                    seed_from_args_ = false; // Fallback to random_device
                }
            }
        }

        if (!seed_from_args_) {
            std::random_device rd;
            seed_ = rd();
            std::cout << "Using seed from random_device: " << seed_ << std::endl;
        }

        gen_.seed(seed_);
    }

    /**
     * @brief Generates a random integer within a specified range (inclusive).
     *
     * @param min The minimum value of the random integer.
     * @param max The maximum value of the random integer.
     * @return A random integer between min and max.
     */
    int nextInt(int min, int max) {
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen_);
    }

    /**
     * @brief Gets the current seed used by the generator.
     *
     * @return The current seed.
     */
    unsigned int getSeed() const {
        return seed_;
    }

    /**
     * @brief Checks if the seed was provided via command line arguments.
     *
     * @return True if the seed was from command line, false otherwise.
     */
    bool isSeedFromArgs() const {
        return seed_from_args_;
    }

    /**
     * @brief Writes the current seed and its source to a file.
     *
     * The file will be named "seed_YYYYMMDD_HHMMSS.txt" and placed in the
     * specified base directory. If the directory does not exist, it will be created.
     *
     * @param base_dir The directory where the seed file will be created.
     */
    void writeSeedToFile(const std::string& base_dir) {
        try {
            // Create the directory if it doesn't exist
            if (!base_dir.empty() && !std::filesystem::exists(base_dir)) {
                std::filesystem::create_directories(base_dir);
            }

            std::string filename = (base_dir.empty() ? "" : base_dir + "/") + "seed_" + get_current_date() + ".txt";
            std::ofstream seed_file(filename);

            if (!seed_file.is_open()) {
                std::cerr << "Error: Could not open seed file for writing: " << filename << std::endl;
                return;
            }

            seed_file << "Seed: " << seed_ << std::endl;
            if (seed_from_args_) {
                seed_file << "Source: Command line argument" << std::endl;
            } else {
                seed_file << "Source: Random device" << std::endl;
            }
            seed_file.close();
            std::cout << "Seed details written to: " << filename << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error creating directory or writing seed file: " << e.what() << std::endl;
        }
    }
};

#endif // GENERATOR_UTILS_HPP
