#include <vector>
#include <istream>
#include <string>

namespace assembler {
  class FileProcessor {
    public:
      // Takes an istream and parameter vector as input. If successful, adds
      // one string representing each *non-whitespace* line to the param vector
      // returns true if successful, false if unexpected error encountered.
      static bool clean_input(std::istream& is, std::vector<std::string>* output);
  };
}
