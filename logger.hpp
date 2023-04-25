#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <fstream>
#include <array>
#include <iomanip>
#include <iostream>

template<size_t Buffer_Size>
class Logger{
private:
    std::string filename_;
    std::array<std::string, Buffer_Size> buffer_;
    size_t index_;

    std::streambuf* coutBuf;
    std::ofstream file;

    void open_file(){
        if(!filename_.empty()){
            file.open(filename_,std::ios::app); // attempt to open file in append mode
            if(file.is_open()){
                coutBuf = std::cout.rdbuf(); // save cout buffer to revert back later
                std::cout.rdbuf(file.rdbuf()); // redirect cout to file
            }
        }
    }

    void close_file(){
        if(!filename_.empty() && file.is_open()){
            std::cout.rdbuf(coutBuf); // restore std::cout
            file.close();
        }
    }
    void flush_buffer(){
        open_file();
        for(int i = 0; i < Buffer_Size; ++i){
            std::cout << std::setprecision(12) << buffer_[i] << std::endl;
        }
    }

public:
    Logger():
        filename_(),
        buffer_{},
        index_(0)
    {
    }

    Logger(const char* filename):
        filename_(filename),
        buffer_{},
        index_(0)
    {
    }

    ~Logger(){
    }

    void set_filename(const char* filename){
        filename_ = filename;
    }

    void log(std::string msg){
        buffer_[index_] = msg;
        index_ = (index_ + 1 ) % Buffer_Size;

        // once it's looped around, flush it!
        if(index_ == 0){
            flush_buffer();
        }
    }


};
#endif // LOGGER_HPP_