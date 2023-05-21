from enum import Enum
from abc import ABC
class InstructionType(Enum):
	A = 1
	C = 2

class Instruction(ABC):
	def get_type(self) -> InstructionType:
		pass

class AInstruction(Instruction):
	def __init__(self, addr):
		self.addr = addr

	def get_type(self):
		return InstructionType.A

	def has_symbols(self):
		return type(self.addr) != int

class CInstruction(Instruction):
	def __init__(self, a_cmp, dst, jmp):
		self.a_cmp = a_cmp
		self.dst = dst
		self.jmp = jmp
	
	def get_type(self):
		return InstructionType.C
