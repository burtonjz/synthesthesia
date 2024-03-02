#include "Filter.hpp"
#include "ModuleType.hpp"

Filter::Filter():
    Filter(nullptr)
{}

Filter::Filter(const double* sampleRate):
    Module(sampleRate,ModuleType::Filter,-1) // TODO: set instance properly
{}

std::pair<const ParameterType*, size_t> Filter::getControlPorts(){}

void Filter::activate(const double* sampleRate){}

void Filter::setOutputBuffer(float* buffer, size_t channel){}

const FilterType Filter::getFilterType(){}

void Filter::setFilterType(FilterType typ){}

// void Filter::calculate_coefficients(){
//     double w0, alpha, cosw0, a0;

//     switch(type_){
//     case FilterType::LOW_PASS:
//         w0 = 2.0 * M_PI * inst_cutoff_freq / (rate/2.0); // normalized by nyquist frequency
//         alpha = std::sin(w0) / (2.0 * inst_q_factor);
//         cosw0 = std::cos(w0);
//         a0 = 1.0 + alpha;
//         b0 = (1.0 - cosw0) / (2.0 * a0);
//         b1 = (1.0 - cosw0) / a0;
//         b2 = b0;
//         a1 = (-2.0 * cosw0) / a0;
//         a2 = (1.0 - alpha) / a0;
//         break;
//     case FilterType::HIGH_PASS:
//         w0 = 2.0 * M_PI * inst_cutoff_freq / (rate/2.0);
//         alpha = std::sin(w0) / (2.0 * inst_q_factor);
//         cosw0 = std::cos(w0);
//         a0 = 1.0 + alpha;
//         b0 = (1.0 + cosw0) / (2.0 * a0);
//         b1 = -(1.0 + cosw0) / a0;
//         b2 = b0;
//         a1 = (-2.0 * cosw0) / a0;
//         a2 = (1.0 - alpha) / a0;
//         break;
//     case FilterType::BAND_PASS:
//         w0 = 2.0 * M_PI * inst_custoff_freq / (rate/2.0);
//         alpha = std::sin(w0) / (2.0 * inst_q_factor);
//         cosw0 = std::cos(w0);
//         a0 
//     default:
//         break;
// }

// void Filter::process(){}



