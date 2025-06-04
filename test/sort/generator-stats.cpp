#include <bits/stdc++.h>

// Get the directory of the current executable
std::string get_code_dir() {
    std::filesystem::path exe_path = std::filesystem::current_path();
    return exe_path.string();
}

std::string CODE_DIR = get_code_dir();

std::set<std::string> file_cleaned;
int count_write = 0;
std::string write_acumulation = "";

// Random number generator setup
std::random_device rd;
unsigned int seed = rd();
std::mt19937 gen(seed);
bool first_randint_call = true;

std::string get_current_date() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

void write_seed_to_file() {
    std::string filename = "seed" + get_current_date() + ".txt";
    std::ofstream seed_file(CODE_DIR + "/" + filename);
    seed_file << "Seed: " << seed << std::endl;
    seed_file.close();
}

int randint(int min, int max) {
    if (first_randint_call) {
        write_seed_to_file();
        first_randint_call = false;
    }
    
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void create_or_clear(const std::string& filename) {
    std::ofstream file(CODE_DIR + "/" + filename + ".in");
    file << "";
    file.close();
}

void write(const std::string& filename, const std::string& content) {
    count_write++;
    count_write %= 1000000;

    write_acumulation += content;

    if (count_write != 0) {
        return;
    }

    if (file_cleaned.find(filename) == file_cleaned.end()) {
        create_or_clear(filename);
        file_cleaned.insert(filename);
    }

    std::ofstream file(CODE_DIR + "/" + filename + ".in", std::ios::app);
    file << write_acumulation;
    file.close();

    write_acumulation = "";
}

int main() {
    for (int n = 1; n < 10; n++) {
        int size = static_cast<int>(std::pow(10, n));

        write(
            "T" + std::to_string(n),
            std::to_string(size) + "\n"
        );

        for (int i = 0; i < size; i++) {
            write(
                "T" + std::to_string(n),
                std::to_string(randint(1, 1000000000)) + (size > i + 1 ? " " : "\n")
            );
        }

        count_write = -1;
        write("T" + std::to_string(n), "");
    }

    return 0;
}