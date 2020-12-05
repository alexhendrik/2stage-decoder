// CEC470_FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

#define HALT_OPCODE 0x19

void fetchNextInstruction(void);
void executeInstruction(void);
void loadMemory(char*, unsigned char*);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;


int main(int argc, char* argv[])
{
    unsigned char testMemory[655356];
    
    char inputFileName[] = "mem_in.txt";
    loadMemory(inputFileName, memory);

    char outputFileName[] = "mem_out.txt";
    loadMemory(outputFileName, testMemory);
    
    int inst = 0;
    while (memory[PC] != HALT_OPCODE)
    {
        try {
            auto flags = cout.flags();
            cout << "| Instruction: " << ++inst << " | PC: " << "0x" << hex << (int)(PC) << " | IR: " << "0x" << hex << (int)(memory[PC]);
            fetchNextInstruction();
            executeInstruction();
            cout << " | ACC: " << "0x" << hex << (int)(ACC) << " | MAR: " << "0x" << hex << (int)(MAR) << "|\n";
            cout.flags(flags); 
        }
        catch (...) {
            cout << inst << "\n";
            throw "something is boof dog";
        }
    }

    for (int i = 0; i < 65536; i++) {
        if (memory[i] != testMemory[i]) {
            cout << i << "\n";
            cout << "Expected:" << (int)testMemory[i] << "\n";
            cout << "Provided:" << (int)memory[i] << "\n";
            //return -1;
        }
    }

    return 0;
}

void fetchNextInstruction(void){
    IR = memory[PC];

    //checks if its a math operator
    if ((IR & 0x80) == 0x80)
    {
        // single/two operand
        if ((IR & 0x70) >> 4 <= 4) {
            switch (IR & 0x0F)
            {
            case 0b0000:
                PC++;
                break;
            case 0b0001:
                PC++;
                break;
            case 0b0010:
                PC += 2;
                break;
            case 0b0011:
                PC += 3;
                break;

            case 0b0100:
                PC++;
                break;
            case 0b0101:
                PC++;
                break;
            case 0b0110:
                PC += 2;
                break;
            case 0b0111:
                PC += 3;
                break;

            case 0b1000:
                PC++;
                break;
            case 0b1001:
                PC++;
                break;
            case 0b1010:
                PC += 2;
                break;
            case 0b1011:
                PC += 3;
                break;

            case 0b1100:
                PC += 3;
                break;
            case 0b1101:
                PC += 3;
                break;
            case 0b1110:
                PC += 4;
                break;
            case 0b1111:
                PC += 5;
                break;
            }
        }
        //incremement/decrement/not on register
        else if ((IR & 0x0C) != 0b1100){
            PC++;
        }
        //increment/decrement/not on memory
        else {
            PC += 3;
        }
    }
    //Check if its a memory function
    else if ((IR & 0xF0) == 0x0)
    {
        //store operation
        if ((IR & 0x08) == 0x0) {
            //ACC
            if ((IR & 0x04) == 0x00) {
                //operand is address
                if ((IR & 0x3) == 0) {
                    //store ACC into the memory location specified by operand
                    PC += 3;
                }
                //MAR is pointer
                else {
                    // dereference MAR and store ACC into it
                    PC++;
                }
            }
            //MAR
            else {
                //operand is address
                if ((IR & 0x3) == 0) {
                    //store the MAR into memory location specified by operand
                    PC += 3;
                }
            }
        }
        //load operation
        else {
            //ACC
            if ((IR & 0x04) == 0x00) {
                //operand is address
                if ((IR & 0x3) == 0) {
                    PC += 3;
                }
                //operand is a constant
                else if ((IR & 0x3) == 1) {
                    PC += 2;
                }
                //MAR is pointer
                else {
                    PC++;
                }
            }
            //MAR
            else {
                //operand is address
                if ((IR & 0x3) == 0) {
                    PC += 3;
                }
                //operand is a constant
                else if ((IR & 0x3) == 1) {
                    PC += 3;
                }
                //MAR is pointer
                else {
                    PC++;
                }
            }
        }
    }
    //check if its a branch function
    else if ((IR & 0xF8) == 0x10)
    {
        PC += 3;
    }
    //Special or illegal opcode
    else
    {
        if (IR == 0x18) // Then it's a NOP --do nothing
        {
            PC++;
        }
        else if (IR == 0x19) // Then it's a HALT --framework will halt for us
        {
        }
        else // Otherwise it's an illegal opcode --you can print an error message if you want to
        {
            throw "Unrecognized operator!";
        }
    }
}

void executeInstruction(void)
{
    //checks if its a math operator
    if ((IR & 0x80) == 0x80) 
    {
        switch ((IR & 0x70) >> 4)
        {
            //AND
            case 0:
                switch (IR & 0x0F)
                {
                    //indirect - MAR is a pointer to destination and MAR is a pointer to source
                    case 0b0000:
                        //This does nothing but is technically legal
                        memory[MAR] &= memory[MAR];
                        PC++;
                        break;
                    // indirect - MAR is a pointer to destination and ACC is source
                    case 0b0001: 
                        memory[MAR] &= ACC;
                        PC++;
                        break;
                    // indirect - MAR is a pointer to destination and constant is source
                    case 0b0010:
                        memory[MAR] &= memory[PC - 1];
                        break;
                    // indirect - MAR is a pointer to destination and memory address is source
                    case 0b0011:
                        memory[MAR] &= memory[(memory[PC -2] <<8) + memory[PC-1]];
                        break;

                    // ACC is destination and MAR is a pointer to source
                    case 0b0100:
                        ACC &= memory[MAR];
                        break;
                    // ACC is destination and ACC is source
                    case 0b0101:
                        //This does nothing but its technically legal
                        ACC &= ACC;
                        break;
                    // ACC is destination and constant is source
                    case 0b0110:                
                        ACC &= memory[PC - 1];
                        break;
                    // ACC is destination and memory address is source
                    case 0b0111:
                        ACC &= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                        break;

                    // MAR is destination and MAR is a pointer to source
                    case 0b1000:
                        MAR &= memory[MAR];
                        break;
                    // MAR is destination and ACC is source
                    case 0b1001: 
                        MAR &= ACC;
                        break;
                    // MAR is destination and constant is source
                    case 0b1010: 
                        MAR &= memory[PC - 1];
                        break;
                    // MAR is destination and memory address is source
                    case 0b1011:
                        MAR &= memory[(memory[PC -2] << 8) + memory[PC -1]];
                        break;

                    // memory address is destination and MAR is a pointer to source
                    case 0b1100: 
                        memory[(memory[PC - 2] << 8) + memory[PC - 1]] &= memory[MAR];
                        break;
                    // memory address is destination and ACC is source
                    case 0b1101:
                        memory[(memory[PC - 2] << 8) + memory[PC -1]] &= ACC;
                        break;
                    // memory address is destination and constant is source
                    case 0b1110:
                        memory[(memory[PC - 3] << 8) + memory[PC - 2]] &= memory[PC - 1];
                        break;
                    // memory address is destination and memory address is source
                    case 0b1111:
                        memory[(memory[PC - 4] << 8) + memory[PC - 3]] &= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                        break;
                }
                break;
            //OR
            case 1:
                switch (IR & 0x0F)
                {
                    //indirect - MAR is a pointer to destination and MAR is a pointer to source
                case 0b0000:
                    //This does nothing but is technically legal
                    memory[MAR] |= memory[MAR];
                    PC++;
                    break;
                    // indirect - MAR is a pointer to destination and ACC is source
                case 0b0001:
                    memory[MAR] |= ACC;
                    PC++;
                    break;
                    // indirect - MAR is a pointer to destination and constant is source
                case 0b0010:
                    memory[MAR] |= memory[PC - 1];
                    break;
                    // indirect - MAR is a pointer to destination and memory address is source
                case 0b0011:
                    memory[MAR] |= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // ACC is destination and MAR is a pointer to source
                case 0b0100:
                    ACC |= memory[MAR];
                    break;
                    // ACC is destination and ACC is source
                case 0b0101:
                    //This does nothing but its technically legal
                    ACC |= ACC;
                    break;
                    // ACC is destination and constant is source
                case 0b0110:
                    ACC |= memory[PC - 1];
                    break;
                    // ACC is destination and memory address is source
                case 0b0111:
                    ACC |= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // MAR is destination and MAR is a pointer to source
                case 0b1000:
                    MAR |= memory[MAR];
                    break;
                    // MAR is destination and ACC is source
                case 0b1001:
                    MAR |= ACC;
                    break;
                    // MAR is destination and constant is source
                case 0b1010:
                    MAR |= memory[PC - 1];
                    break;
                    // MAR is destination and memory address is source
                case 0b1011:
                    MAR |= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // memory address is destination and MAR is a pointer to source
                case 0b1100:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] |= memory[MAR];
                    break;
                    // memory address is destination and ACC is source
                case 0b1101:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] |= ACC;
                    break;
                    // memory address is destination and constant is source
                case 0b1110:
                    memory[(memory[PC - 3] << 8) + memory[PC - 2]] |= memory[PC - 1];
                    break;
                    // memory address is destination and memory address is source
                case 0b1111:
                    memory[(memory[PC - 4] << 8) + memory[PC - 3]] |= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;
                }
                break;
            //XOR
            case 2:
                switch (IR & 0x0F)
                {
                    //indirect - MAR is a pointer to destination and MAR is a pointer to source
                case 0b0000:
                    //This does nothing but is technically legal
                    memory[MAR] ^= memory[MAR];
                    PC++;
                    break;
                    // indirect - MAR is a pointer to destination and ACC is source
                case 0b0001:
                    memory[MAR] ^= ACC;
                    PC++;
                    break;
                    // indirect - MAR is a pointer to destination and constant is source
                case 0b0010:
                    memory[MAR] ^= memory[PC - 1];
                    break;
                    // indirect - MAR is a pointer to destination and memory address is source
                case 0b0011:
                    memory[MAR] ^= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // ACC is destination and MAR is a pointer to source
                case 0b0100:
                    ACC ^= memory[MAR];
                    break;
                    // ACC is destination and ACC is source
                case 0b0101:
                    //This does nothing but its technically legal
                    ACC ^= ACC;
                    break;
                    // ACC is destination and constant is source
                case 0b0110:
                    ACC ^= memory[PC - 1];
                    break;
                    // ACC is destination and memory address is source
                case 0b0111:
                    ACC ^= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // MAR is destination and MAR is a pointer to source
                case 0b1000:
                    MAR ^= memory[MAR];
                    break;
                    // MAR is destination and ACC is source
                case 0b1001:
                    MAR ^= ACC;
                    break;
                    // MAR is destination and constant is source
                case 0b1010:
                    MAR ^= memory[PC - 1];
                    break;
                    // MAR is destination and memory address is source
                case 0b1011:
                    MAR ^= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // memory address is destination and MAR is a pointer to source
                case 0b1100:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] ^= memory[MAR];
                    break;
                    // memory address is destination and ACC is source
                case 0b1101:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] ^= ACC;
                    break;
                    // memory address is destination and constant is source
                case 0b1110:
                    memory[(memory[PC - 3] << 8) + memory[PC - 2]] ^= memory[PC - 1];
                    break;
                    // memory address is destination and memory address is source
                case 0b1111:
                    memory[(memory[PC - 4] << 8) + memory[PC - 3]] ^= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;
                }
                break;
            //ADD
            case 3:
                switch (IR & 0x0F)
                {
                    //indirect - MAR is a pointer to destination and MAR is a pointer to source
                case 0b0000:
                    //This does nothing but is technically legal
                    memory[MAR] += memory[MAR];
                    PC++;
                    break;
                    // indirect - MAR is a pointer to destination and ACC is source
                case 0b0001:
                    memory[MAR] += ACC;
                    PC++;
                    break;
                    // indirect - MAR is a pointer to destination and constant is source
                case 0b0010:
                    memory[MAR] += memory[PC - 1];
                    break;
                    // indirect - MAR is a pointer to destination and memory address is source
                case 0b0011:
                    memory[MAR] += memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // ACC is destination and MAR is a pointer to source
                case 0b0100:
                    ACC += memory[MAR];
                    break;
                    // ACC is destination and ACC is source
                case 0b0101:
                    //This does nothing but its technically legal
                    ACC += ACC;
                    break;
                    // ACC is destination and constant is source
                case 0b0110:
                    ACC += memory[PC - 1];
                    break;
                    // ACC is destination and memory address is source
                case 0b0111:
                    ACC += memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // MAR is destination and MAR is a pointer to source
                case 0b1000:
                    MAR += memory[MAR];
                    break;
                    // MAR is destination and ACC is source
                case 0b1001:
                    MAR += ACC;
                    break;
                    // MAR is destination and constant is source
                case 0b1010:
                    MAR += memory[PC - 1];
                    break;
                    // MAR is destination and memory address is source
                case 0b1011:
                    MAR += memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;

                    // memory address is destination and MAR is a pointer to source
                case 0b1100:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] += memory[MAR];
                    break;
                    // memory address is destination and ACC is source
                case 0b1101:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] += ACC;
                    break;
                    // memory address is destination and constant is source
                case 0b1110:
                    memory[(memory[PC - 3] << 8) + memory[PC - 2]] += memory[PC - 1];
                    break;
                    // memory address is destination and memory address is source
                case 0b1111:
                    memory[(memory[PC - 4] << 8) + memory[PC - 3]] += memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;
                }
                break;
            //SUB
            case 4: 
                switch (IR & 0x0F)
            {
                //indirect - MAR is a pointer to destination and MAR is a pointer to source
            case 0b0000:
                //This does nothing but is technically legal
                memory[MAR] -= memory[MAR];
                PC++;
                break;
                // indirect - MAR is a pointer to destination and ACC is source
            case 0b0001:
                memory[MAR] -= ACC;
                PC++;
                break;
                // indirect - MAR is a pointer to destination and constant is source
            case 0b0010:
                memory[MAR] -= memory[PC - 1];
                break;
                // indirect - MAR is a pointer to destination and memory address is source
            case 0b0011:
                memory[MAR] -= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                break;

                // ACC is destination and MAR is a pointer to source
            case 0b0100:
                ACC -= memory[MAR];
                break;
                // ACC is destination and ACC is source
            case 0b0101:
                //This does nothing but its technically legal
                ACC -= ACC;
                break;
                // ACC is destination and constant is source
            case 0b0110:
                ACC -= memory[PC - 1];
                break;
                // ACC is destination and memory address is source
            case 0b0111:
                ACC -= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                break;

                // MAR is destination and MAR is a pointer to source
            case 0b1000:
                MAR -= memory[MAR];
                break;
                // MAR is destination and ACC is source
            case 0b1001:
                MAR -= ACC;
                break;
                // MAR is destination and constant is source
            case 0b1010:
                MAR -= memory[PC - 1];
                break;
                // MAR is destination and memory address is source
            case 0b1011:
                MAR -= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                break;

                // memory address is destination and MAR is a pointer to source
            case 0b1100:
                memory[(memory[PC - 2] << 8) + memory[PC - 1]] -= memory[MAR];
                break;
                // memory address is destination and ACC is source
            case 0b1101:
                memory[(memory[PC - 2] << 8) + memory[PC - 1]] -= ACC;
                break;
                // memory address is destination and constant is source
            case 0b1110:
                memory[(memory[PC - 3] << 8) + memory[PC - 2]] -= memory[PC - 1];
                break;
                // memory address is destination and memory address is source
            case 0b1111:
                memory[(memory[PC - 4] << 8) + memory[PC - 3]] -= memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                break;
            }
                break;
            //INC
            case 5:
                switch (IR & 0x0C)
                {
                //indirect - MAR is a pointer to destination
                case 0b0000:
                    memory[MAR]++;
                    break;

                // ACC is destination
                case 0b0100:
                    ACC ++;
                    break;

                // MAR is destination
                case 0b1000:
                    MAR++;
                    break;

                // memory address is destination
                case 0b1100:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]]++;
                    break;
                }
                break;
            //DEC
            case 6:
                switch (IR & 0x0C)
                {
                    //indirect - MAR is a pointer to destination
                case 0b0000:
                    memory[MAR]--;
                    break;

                    // ACC is destination
                case 0b0100:
                    ACC--;
                    break;

                    // MAR is destination
                case 0b1000:
                    MAR--;
                    break;

                    // memory address is destination
                case 0b1100:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]]--;
                    break;
                }
                break;
            //NOT
            case 7:
                switch (IR & 0x0C)
                {
                    //indirect - MAR is a pointer to destination
                case 0b0000:
                    memory[MAR] = !memory[MAR];
                    break;

                    // ACC is destination
                case 0b0100:
                    ACC = !ACC;
                    break;

                    // MAR is destination
                case 0b1000:
                    MAR = !MAR;
                    break;

                    // memory address is destination
                case 0b1100:
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] = !memory[(memory[PC - 2] << 8) + memory[PC - 1]];
                    break;
                }
                break;
        }
    }
    //Check if its a memory function
    else if ((IR & 0xF0) == 0x0) 
    {
        //store operation
        if ((IR & 0x08) == 0x0) {
            //ACC
            if ((IR & 0x04) == 0x00) {
                //operand is address
                if ((IR & 0x3) == 0) {
                    //store ACC into the memory location specified by operand
                    memory[(memory[PC - 2] << 8) + memory[PC-1]] = ACC;
                }
                //operand is a constant
                else if ((IR & 0x3) == 1) {
                    throw "Opcode doesnt make sense. Cant store into a constant, can only store into an address";
                }
                //MAR is pointer
                else {
                    // dereference MAR and store ACC into it
                    memory[MAR] = ACC;
                }
            }
            //MAR
            else {
                //operand is address
                if ((IR & 0x3) == 0) {
                    //store the MAR into memory location specified by operand
                    memory[(memory[PC - 2] << 8) + memory[PC - 1]] = (MAR & 0xFF00) >> 8;
                    memory[(memory[PC - 2] << 8) + memory[PC - 1] + 1] = (MAR & 0xFF);
                }
                //operand is a constant
                else if ((IR & 0x3) == 1) {
                    throw "Opcode doesnt make sense. Cant store into a constant, can only store into an address";
                }
                //MAR is pointer
                else {
                    throw "Opcode doesnt make sense. Cant store into a constant, can only store into an address";
                }
            }
        }
        //load operation
        else {
            //ACC
            if ((IR & 0x04) == 0x00) {
                //operand is address
                if ((IR & 0x3) == 0) {
                    //load the next 8 bits (1 sets of memory instruction) into ACC, but its an address
                    ACC = memory[(memory[PC - 2] << 8) + memory[PC-1]];
                }
                //operand is a constant
                else if ((IR & 0x3) == 1) {
                    //load the next 8 bits (1 sets of memory instruction) into ACC, but its a constant
                    ACC = memory[PC-1];
                }
                //MAR is pointer
                else {
                    // dereference MAR and store into ACC
                    ACC = memory[MAR];
                }
            }
            //MAR
            else {
                //operand is address
                if ((IR & 0x3) == 0) {
                    //load the next 16 bits (2 sets of memory instruction) into MAR, but its an address
                    MAR = (memory[(memory[PC - 2] << 8) + memory[PC - 1]]) << 8;
                    MAR += (memory[(memory[PC - 2] << 8) + memory[PC - 1] + 1]);
                }
                //operand is a constant
                else if ((IR & 0x3) == 1) {
                    //load the next 16 bits (2 sets of memory instruction) into MAR, but its a constant
                    MAR = (memory[PC - 2] << 8) + memory[PC - 1];
                }
                //MAR is pointer
                else {
                    // dereference MAR and store into MAR
                    MAR = memory[MAR];
                }
            }
        }
    }
    //check if its a branch function
    else if ((IR & 0xF8) == 0x10) 
    {
        switch (IR & 0x07)
        {
        case 0:
            PC = (memory[PC - 2] << 8) + memory[PC - 1];
            break;
        case 1:
            PC = ACC == 0 ? (memory[PC - 2] << 8) + memory[PC - 1] : PC;
            break;
        case 2:
            PC = ACC != 0 ? (memory[PC - 2] << 8) + memory[PC - 1] : PC;
            break;
        case 3:
            PC = ACC < 0 ? (memory[PC - 2] << 8) + memory[PC - 1]: PC;
            break;
        case 4:
            PC = ACC <= 0 ? (memory[PC - 2] << 8) + memory[PC - 1] : PC;
            break;
        case 5:
            PC = ACC > 0 ? (memory[PC - 2] << 8) + memory[PC - 1] : PC;
            break;
        case 6:
            PC = ACC >= 0 ? (memory[PC - 2] << 8) + memory[PC - 1] : PC;
            break;
        }
    }
    //Special or illegal opcode
    else
    {
        if (IR == 0x18) // Then it's a NOP --do nothing
        {
        }
        else if(IR == 0x19) // Then it's a HALT --framework will halt for us
        {
        }
        else // Otherwise it's an illegal opcode --you can print an error message if you want to
        {
            throw "Unrecognized operator!";
        }
    }
}

void loadMemory(char* file, unsigned char* memory)
{
    ifstream ReadFile(file);
    string line;
    char* res = new char[65536];
    char inst_buffer[2];
    while (getline(ReadFile, line)) 
    {
        unsigned char* memoryStart = memory;
        for (int i = 0; i < line.length(); i += 3) {
            size_t length = line.copy(inst_buffer, 2, i);
            *memory = (unsigned char)strtoul(inst_buffer, NULL, 16);
            memory++;
        }
    }
}
