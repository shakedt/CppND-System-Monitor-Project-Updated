#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    return std::stoi(LinuxParser::CpuUtilization()[0]); // value should be 0.xx find out how to do that
}