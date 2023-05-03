#include <sndfile.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>

int main(int argc, char** argv) {
    // handle cmdline args
    if (argc < 2){
        std::cout << "Usage: ./extract_sound <file_name>" << std::endl;
        return 1;
    }

    std::filesystem::path file_path{argv[1]};
    if (!std::filesystem::exists(file_path)) {
        std::cerr << "File " << file_path << " does not exist\n" << std::endl;
        return 1;
    }
    
    // open WAV file
    SF_INFO info;
    SNDFILE* file = sf_open(file_path.c_str(), SFM_READ,&info);
    if(!file){
        std::cerr << " file is not valid or could not be opened. Exiting. " << std::endl;
        return 1;
    }
    // allocate buffer
    const int BUFFER_SIZE = 1024;
    std::vector<short> buffer(BUFFER_SIZE * info.channels);

    // read audio data
    std::vector<std::vector<short>> audio_data;
    for (int i = 0; i < info.channels; ++i){
        audio_data.emplace_back();
    }
    while (true){
        const int n_samples_read = sf_read_short(file,buffer.data(), BUFFER_SIZE);
        if (n_samples_read == 0){
            break;
        }
        for (int i = 0; i < n_samples_read; ++i){
            for (int j = 0; j < info.channels; ++j ){
                audio_data[j].push_back(buffer[i * info.channels + j]);
            }
        }
    }

    // close file
    sf_close(file);

    // clean up empty sound from start of vectors
    for (int i = 0; i < info.channels; ++i){
        audio_data[i].erase(audio_data[i].begin(), std::find_if(audio_data[i].begin(), audio_data[i].end(), [](short n) { return n != 0; }));
    }

    // just read first channel
    for(short x : audio_data[0]){
        std::cout << x << " ";
    }
    std::cout << std::endl;
    

}