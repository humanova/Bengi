#include "VM.h"


VM::VM(IOFile &iofile)
{
	AllocateSegmentMemory();
	GetSrc(iofile.sourceFile);
	BP = SP = (uint32_t *)((uint32_t)stack + poolsize);
	AX = 0;
	Test();
}


void VM::AllocateSegmentMemory()
{
	if (!(src = old_src = new char[poolsize * 4]))
	{
		BengiError(MALLOC_ERROR_SRC_SEGMENT);
	}
	if (!(text = dump_text = new uint32_t[poolsize]))
	{
		BengiError(MALLOC_ERROR_TEXT_SEGMENT);
	}
	if (!(data = new char[poolsize * 4]))
	{
		BengiError(MALLOC_ERROR_DATA_SEGMENT);
	}
	if (!(stack = new uint32_t[poolsize]))
	{
		BengiError(MALLOC_ERROR_STACK_SEGMENT);
	}

	memset(text, 0, poolsize);
	memset(data, 0, poolsize);
	memset(stack, 0, poolsize);
}

void VM::GetSrc(FILE* &src_file)
{
	fseek(src_file, 0, SEEK_END);
	int fileSize = ftell(src_file);
	rewind(src_file);

	fread(src, 1, fileSize, src_file);
	//int res = fread(src, 1, fileSize, src_file);
	/*
	printf("res = %d, fSize = %d", res, fileSize);
	if (res != fileSize)
	{
		VMError(SOURCE_READING_ERROR, 0);
		exit(-1);
	}*/
	fclose(src_file);
}

void VM::Test()
{
	int i = 0;
	src[i++] = IMM;
	src[i++] = 10;
	src[i++] = PUSH;
	src[i++] = IMM;
	src[i++] = 20;
	src[i++] = ADD;
	src[i++] = PUSH;
	src[i++] = EXIT;
	IP = (uint32_t *)src;

	Eval();
}

void VM::Eval()
{
	int op_id = 0;
	while (1)
	{
		//Get next op
		printf("[%x]", IP);
		op = *IP++; 
		printf(" %d\n", op);

		//MOV instructions
		if (op == IMM)
		{
			AX = *IP++;
		}
		else if (op == LC)
		{
			AX = *(char *)AX;
		}
		else if (op == LI)
		{
			AX = *(uint32_t *)AX;
		}
		else if (op == SC)
		{
			*(char *)*SP++ = AX;
		}
		else if (op == SI)
		{
			*(int *)*SP++ = AX;
		}
		else if (op == PUSH)
		{
			*--SP = AX;
		}

		//Jump instruction
		else if (op == JMP)
		{
			IP = (uint32_t *)*IP;
		}

		//If-else instructions
		else if (op == JZ)
		{
			IP = AX ? IP + 1 : (uint32_t *)*IP;
		}
		else if (op == JNZ)
		{
			IP = AX ? (uint32_t *)*IP : IP + 1;
		}

		//Function instructions
		else if (op == CALL)
		{
			*--SP = (uint32_t)(IP + 1);
			IP = (uint32_t *)*IP;
		}
		else if (op == ENT)
		{
			*--SP = (uint32_t)BP;
			BP = SP;
			SP = SP - *IP++;
		}
		else if (op == ADJ)
		{
			SP = SP + *IP++;
		}
		else if (op == LEV)
		{
			SP = BP; BP = (uint32_t *)*SP++; IP = (uint32_t *)*SP++;
		}
		else if (op == LEA)
		{
			AX = (uint32_t)(BP + *IP++);
		}

		//Mathematical instructions
		else if (op == OR)  AX = *SP++ | AX;
		else if (op == XOR) AX = *SP++ ^ AX;
		else if (op == AND) AX = *SP++ & AX;
		else if (op == EQ)  AX = *SP++ == AX;
		else if (op == NE)  AX = *SP++ != AX;
		else if (op == LT)  AX = *SP++ < AX;
		else if (op == LE)  AX = *SP++ <= AX;
		else if (op == GT)  AX = *SP++ > AX;
		else if (op == GE)  AX = *SP++ >= AX;
		else if (op == SHL) AX = *SP++ << AX;
		else if (op == SHR) AX = *SP++ >> AX;
		else if (op == ADD) AX = *SP++ + AX;
		else if (op == SUB) AX = *SP++ - AX;
		else if (op == MUL) AX = *SP++ * AX;
		else if (op == DIV) AX = *SP++ / AX;
		else if (op == MOD) AX = *SP++ % AX;

		//Built-in instructions
		else if (op == EXIT)
		{
			printf("exit with : (%d)", *SP);
			exit(*SP);
		}
		/*
		else if (op == OPEN)
		{
			AX = open((char *)SP[1], SP[0]);
		}
		else if (op == CLOS)
		{
			AX = close(*SP);
		}
		else if (op == READ)
		{
			AX = read(SP[2], (char *)SP[1], *SP);
		}
		else if (op == PRTF)
		{
			tmp_op = SP + IP[1];
			AX = printf((char *)tmp_op[-1], tmp_op[-2], tmp_op[-3], tmp_op[-4], tmp_op[-5], tmp_op[-6]);
		}*/
		else if (op == MALC) { AX = (int)malloc(*SP); }
		else if (op == MSET) { AX = (int)memset((char *)SP[2], SP[1], *SP); }
		else if (op == MCMP) { AX = memcmp((char *)SP[2], (char *)SP[1], *SP); }
		else
		{
			printf("unknown inst : id : %d \nop : %d", op_id, op);
			//VMError(VM_UNKNOWN_INSTRUCTION, op);
			exit(-1);
		}
	}
}
