import instruction
from instruction import Instruction
from instruction import InstructionType
from instruction import AInstruction
from instruction import CInstruction

def translate_instruction(instruction: Instruction) -> str:
	instruction_type = instruction.get_type()
	if instruction_type == InstructionType.A:
		return translate_a_instruction(instruction)
	elif instruction_type == InstructionType.C:
		return translate_c_instruction(instruction)
	else:
		raise Exception('unrecognized argument type')

def translate_a_instruction(a_instruction: AInstruction) -> str:
	# can't translate something with symbols
	assert(not a_instruction.has_symbols()) 

	# convert addr to binary 
	binary_address = f'{a_instruction.addr:b}'
	for i in range(len(binary_address), 15):
		binary_address = '0' + binary_address

	# adding opcode
	binary_address = '0' + binary_address

	return binary_address

def translate_a_cmp(cmp: str) -> str:
	if cmp == '0':
		return '0101010'
	elif cmp == '1':
		return '0111111'
	elif cmp == '-1':
		return '0111010'
	elif cmp == 'D':
		return '0001100'
	elif cmp == 'A':
		return '0110000'
	elif cmp == 'M':
		return '1110000'
	elif cmp == '!D':
		return '0001101'
	elif cmp == '!A':
		return '0110001'
	elif cmp == '!M':
		return '1110001'
	elif cmp == '-D':
		return '0001111'
	elif cmp == '-A':
		return '0110011'
	elif cmp == '-M':
		return '1110011'
	elif cmp == 'D+1':
		return '0011111'
	elif cmp == 'A+1':
		return '0110111'
	elif cmp == 'M+1':
		return '1110111'
	elif cmp == 'D-1':
		return '0001110'
	elif cmp == 'A-1':
		return '0110010'
	elif cmp == 'M-1':
		return '1110010'
	elif cmp == 'D+A':
		return '0000010'
	elif cmp == 'D+M':
		return '1000010'
	elif cmp == 'D-A':
		return '0010011'
	elif cmp == 'D-M':
		return '1010011'
	elif cmp == 'A-D':
		return '0000111'
	elif cmp == 'M-D':
		return '1000111'
	elif cmp == 'D&A':
		return '0000000'	
	elif cmp == 'D&M':
		return '1000000'	
	elif cmp == 'D|A':
		return '0010101'
	elif cmp == 'D|M':
		return '1010101'
	else:
		raise Exception('cmp not recognized')

def translate_jmp(jmp: str) -> str:
	if jmp is None:
		return '000'
	elif jmp == 'JGT':
		return '001'
	elif jmp == 'JEQ':
		return '010'
	elif jmp == 'JGE':
		return '011'
	elif jmp == 'JLT':
		return '100'
	elif jmp == 'JNE':
		return '101'
	elif jmp == 'JLE':
		return '110'
	elif jmp == 'JMP':
		return '111'
	else:
		raise Exception('jmp not recognized')

def translate_dst(dst: str) -> str:
	if dst == None:
		return '000'
	elif dst == 'M':
		return '001'
	elif dst == 'D':
		return '010'
	elif dst == 'MD':
		return '011'
	elif dst == 'A':
		return '100'
	elif dst == 'AM':
		return '101'
	elif dst == 'AD':
		return '110'
	elif dst == 'AMD':
		return '111'
	else:
		raise Exception('dst not recognized')

def translate_c_instruction(c_instruction: CInstruction) -> str:
	translated = translate_a_cmp(c_instruction.a_cmp)
	translated = translated + translate_dst(c_instruction.dst)
	translated = translated + translate_jmp(c_instruction.jmp)
	translated = '111' + translated
	return translated
