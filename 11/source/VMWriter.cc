#include "VMWriter.h"

#include <iostream>

namespace AbhishekJackCompiler {

using std::ostream;
using std::cerr;

VMWriter::VMWriter(std::ostream* o_stream) : o_stream_(o_stream) {}

void VMWriter::writePush(Segment segment, int index) {
  string seg;
  switch (segment) {
    case Segment::CONSTANT:
      seg = "constant";
      break;
    case Segment::ARGUMENT:
      seg = "argument";
      break;
    case Segment::LOCAL:
			seg = "local";
			break;
    case Segment::STATIC:
			seg = "static";
			break;
    case Segment::THIS:
			seg = "this";
			break;
    case Segment::THAT:
			seg = "that";
			break;
    case Segment::POINTER:
			seg = "pointer";
			break;
    case Segment::TEMP:
			seg = "temp";
			break;
  }

  *o_stream_ << "push " << seg << " " << index << "\n";
}

void VMWriter::writePop(Segment segment, int index) {
  string seg;
  switch (segment) {
    case Segment::CONSTANT:
      cerr << "ERROR: got a constant for a pop statement";
      break;
    case Segment::ARGUMENT:
      seg = "argument";
      break;
    case Segment::LOCAL:
			seg = "local";
			break;
    case Segment::STATIC:
			seg = "static";
			break;
    case Segment::THIS:
			seg = "this";
			break;
    case Segment::THAT:
			seg = "that";
			break;
    case Segment::POINTER:
			seg = "pointer";
			break;
    case Segment::TEMP:
			seg = "temp";
			break;
  }

  *o_stream_ << "pop " << seg << " " << index << "\n";
}

void VMWriter::writeArithmetic(Command command) {
  string cmd;
  switch (command) {
    case Command::ADD:
			cmd = "add";
			break;
    case Command::SUB:
			cmd = "sub";
			break;
    case Command::NEG:
			cmd = "neg";
			break;
    case Command::EQ:
			cmd = "eq";
			break;
    case Command::GT:
			cmd = "gt";
			break;
    case Command::LT:
			cmd = "lt";
			break;
    case Command::AND:
			cmd = "and";
			break;
    case Command::OR:
			cmd = "or";
			break;
    case Command::NOT:
			cmd = "not";
			break;
  }

  *o_stream_ << cmd << "\n";
}

void VMWriter::writeLabel(string label) {
  *o_stream_ << "label " << label << "\n";
}

void VMWriter::writeGoto(string label) {
  *o_stream_ << "goto " << label << "\n";
}

void VMWriter::writeIf(string label) {
  *o_stream_ << "if-goto " << label << "\n";
}

void VMWriter::writeCall(string name, int nArgs) {
  *o_stream_ << "call " << name << " " << nArgs << "\n";
}

void VMWriter::writeFunction(string name, int nVars) {
  *o_stream_ << "function " << name << " " << nVars << "\n";
}

void VMWriter::writeReturn() {
  *o_stream_ << "return\n";
}

}
