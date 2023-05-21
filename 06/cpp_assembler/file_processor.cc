#include "file_processor.h"

#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace assembler {

bool FileProcessor::clean_input(
  std::istream& is,
  std::vector<std::string>* output) {
  for (std::string line; std::getline(is, line); ) {
    std::ostringstream os;
    for (int i = 0; i < line.length(); i++) {
      if (line[i] == '/') {
        if (i < line.length() - 1 && line[i+1] == '/') {
          break;
        } else {
          // This is badly-formatted (one forward slash alone).
          return false;
        }
      } else if (isspace(line[i])) {
        continue;
      }
      os << line[i];
    }
    // leave out whitespace lines
    if (os.str().length() > 0) {
      output->push_back(os.str());
    }
  }
  // return true if no errors found during processing
  return true;
}

}
