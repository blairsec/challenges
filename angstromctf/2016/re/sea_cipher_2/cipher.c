#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/* State IDs */
#define STATE_INITVARS	0
#define STATE_ALLOCBUF	1	// alloc a buffer (rotors or key)
#define STATE_FILLBUF	2	// fill out a buffer - 2 for buf0, 3 for buf1
#define STATE_ROTATE	4	// rotate buffer CW
#define STATE_SHIFT		5	// shift buffer left by 1
#define STATE_FROB		6	// frobnicate (XOR) buffer
#define STATE_OPENFILE	7	// open a file
#define STATE_READCHAR	8	// read a character from the input file
#define STATE_PARSECHAR	9	// perform the encryption
#define STATE_WRITECHAR	10	// write a parsed character to the output file
#define STATE_INCREMENT	11	// increment the key

/* Pointer to char_idx */
int *dbgglobal = (int*)0x4f593872;

/* Initial rotors state */
char init_rotors[] = {63, 53, 40, 35, 61, 42, 36, 60, 34, 59, 32, 56, 46, 33, 52, 45, 57, 54, 41, 47, 39, 37, 62, 58, 38, 43,
					  37, 59, 34, 32, 63, 47, 52, 36, 33, 46, 43, 40, 41, 56, 57, 42, 58, 61, 53, 54, 38, 60, 35, 39, 45, 62,
					  52, 40, 35, 34, 53, 57, 47, 54, 42, 60, 45, 58, 43, 38, 33, 61, 63, 56, 41, 36, 62, 59, 37, 39, 46, 32,
					  35, 34, 47, 58, 42, 60, 56, 57, 33, 43, 38, 53, 32, 37, 36, 52, 63, 40, 54, 45, 59, 61, 62, 41, 39, 46,
					  52, 34, 33, 62, 32, 39, 40, 37, 36, 54, 41, 43, 63, 60, 46, 38, 57, 35, 56, 53, 58, 47, 61, 42, 59, 45,
					  32, 40, 41, 63, 57, 56, 46, 45, 42, 59, 39, 52, 37, 53, 62, 33, 34, 43, 61, 36, 35, 47, 58, 38, 54, 60,
					  57, 63, 38, 32, 62, 52, 35, 61, 54, 41, 34, 43, 33, 39, 46, 56, 40, 47, 58, 42, 37, 60, 45, 53, 59, 36,
					  43, 47, 32, 60, 59, 37, 41, 35, 62, 57, 52, 53, 54, 61, 34, 33, 42, 46, 56, 38, 63, 36, 45, 39, 58, 40,
					  33, 32, 52, 34, 45, 62, 58, 42, 47, 36, 41, 40, 63, 54, 35, 39, 46, 38, 53, 59, 57, 37, 56, 43, 60, 61};

/* Initial key state */
char init_key[] = {76, 76, 76, 76, 76, 76, 76, 76, 76};

int encrypt_sm(char *in_name, char *out_name)
{
	/* Buffers from the heap */
	uint8_t *rotors, *key;
	uint8_t **buf_addrs[2];
	size_t buf_sizes[2];
	int cur_buf_alloc;

	/* Buffer fill state */
	int ncycles;		// number of rotate-shift-frob cycles
	int op, op_ret;		// operation state and what state it returns to
	size_t rows, cols;	// rows and columns of buffer
	uint8_t xor_var;	// variable to XOR with

	/* File I/O vars */
	FILE *in, *out;
	FILE **fp_addrs[2];
	char *paths[2];
	char mode;
	int cur_fp;

	/* Encryption state */
	int char_idx;
	char read_char;
	char curr;
	int round;

	/* Increment state */
	int inc_idx;

	/* Continually execute states */
	int state = STATE_INITVARS;
	while(1)
	{
		switch(state)
		{
		/* Initialize state machine variables */
		case STATE_INITVARS: ;
			/* Buffer allocation vars */
			buf_addrs[0] = &rotors; buf_addrs[1] = &key;
			buf_sizes[0] = 26 * 9; buf_sizes[1] = (buf_sizes[0] << 1) / 52;
			cur_buf_alloc = 0;

			/* Buffer fill state */
			ncycles = 0;
			op = 0; op_ret = STATE_FILLBUF + 0;
			rows = buf_sizes[1], cols = buf_sizes[0] / buf_sizes[1];
			xor_var = 0x20;

			/* Set up signal handling stuff */
			if (in_name != (char*)SIGTRAP)
			{
				signal(SIGTRAP, (void (*)(int))&encrypt_sm);

				char_idx = (((int)time(NULL)) & ~1) + 1;
				dbgglobal = &char_idx;
			}

			/* Next state, allocate them */
			state = STATE_ALLOCBUF;
			break;
		/* Allocate a buffer */
		case STATE_ALLOCBUF: ;
			/* Make the allocation, failing if necessary */
			uint8_t *buf = malloc(buf_sizes[cur_buf_alloc]);
			if (!buf) return -1;
			*buf_addrs[cur_buf_alloc] = buf;

			/* Next state, fill out the buffer */
			state = STATE_FILLBUF + cur_buf_alloc;
			break;
		/* Fill out the rotors array */
		case STATE_FILLBUF + 0: ;
			/* Set up the initial state of the rotors fillbuf cmd */
			if (op == 0)
			{
				/* In signal handler, so set char_idx back to a multiple of 2 and return */
				if (in_name == (char*)SIGTRAP)
				{
					*dbgglobal = *dbgglobal & ~1;
					free(*buf_addrs[cur_buf_alloc]);
					return 0;
				}

				/* Set up the initial bytes of the rotors buffer */
				memcpy(*buf_addrs[cur_buf_alloc], init_rotors, buf_sizes[cur_buf_alloc]);

				/* First operation */
				op = STATE_ROTATE - 1;
			}

			/* Current state is the frob, so reset */
			if (op == STATE_FROB)
			{
				/* Next cycle, and reset op */
				ncycles++;
				op = STATE_ROTATE - 1;

				/* If 20 cycles have passed, the rotors are decoded */
				if (ncycles == 20)
				{
					cur_buf_alloc++;
					ncycles = 0;
					op = 0;
					cols = rows;
					rows = 1;
					xor_var = 0x20;
					state = STATE_ALLOCBUF;
					break;
				}
			}

			/* Next operation */
			op++; op_ret = STATE_FILLBUF + 0;
			state = op;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				char_idx += 2;
			}

			break;
		/* Fill out the key array */
		case STATE_FILLBUF + 1: ;
			/* Set up the initial state of the keys fillbuf cmd */
			if (op == 0)
			{
				/* Set up the initial bytes of the keys buffer */
				memcpy(*buf_addrs[cur_buf_alloc], init_key, buf_sizes[cur_buf_alloc]);

				/* First operation */
				op = STATE_ROTATE - 1;
			}

			/* Current state is the frob, so reset */
			if (op == STATE_FROB)
			{
				/* Next cycle, and reset op */
				ncycles++;
				op = STATE_ROTATE - 1;

				/* If 20 cycles have passed, the key is decoded */
				if (ncycles == 20)
				{
					fp_addrs[0] = &in; fp_addrs[1] = &out;
					paths[0] = in_name; paths[1] = out_name;
					mode = 'r';

					state = STATE_OPENFILE;
					break;
				}
			}

			/* Next operation */
			op++; op_ret = STATE_FILLBUF + 1;
			state = op;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				key[(char_idx + 13) % 9] = (key[(char_idx + 13) % 9] - 12) % 26;
				key[(char_idx + 20) % 9] = (key[(char_idx + 20) % 9] - 1) % 26;
				key[(char_idx + 16) % 9] = (key[(char_idx + 16) % 9] - 4) % 26;
				char_idx += 2;
			}

			break;
		/* Rotate a buffer clockwise */
		case STATE_ROTATE: ;
			/* Make a copy on the heap */
			uint8_t *copy = (uint8_t*)malloc(buf_sizes[cur_buf_alloc]);
			memcpy(copy, *buf_addrs[cur_buf_alloc], buf_sizes[cur_buf_alloc]);

			/* Copy each row of the original into a column */
			uint8_t *new = *buf_addrs[cur_buf_alloc];
			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					new[(j * rows) + (rows - i - 1)] = copy[(i * cols) + j];
				}
			}

			/* Free copy */
			free(copy);

			/* Invert rows and columns */
			int tmp = rows;
			rows = cols;
			cols = tmp;

			/* Next operation */
			state = op_ret;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				char_idx += 2;
			}

			break;
		/* Shift a buffer left by 1 */
		case STATE_SHIFT: ;
			/* Shift each row */
			for (size_t i = 0; i < rows; i++)
			{
				uint8_t *row = *buf_addrs[cur_buf_alloc] + (cols * i);

				/* Check for debugger with signals */
				char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
				__asm__("int3");
				if (char_idx % 2 == 1)
				{
					rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
					rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
					rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
					char_idx += 2;
				}

				uint8_t tmp = row[0];
				memcpy(row, row + 1, cols - 1);
				row[cols - 1] = tmp;
			}

			/* Next operation */
			state = op_ret;
			break;
		/* Frobnicate a buffer */
		case STATE_FROB: ;
			/* XOR entire buf */
			buf = *buf_addrs[cur_buf_alloc];
			for (size_t i = 0; i < buf_sizes[cur_buf_alloc]; i++) buf[i] = buf[i] ^ xor_var;

			/* Change XOR var */
			xor_var += 7;

			/* Next operation */
			state = op_ret;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				char_idx += 2;
			}

			break;
		/* Open a file */
		case STATE_OPENFILE: ;
			/* Open the file, returning on failure */
			char modestr[2] = {mode, 0};
			FILE *fp = fopen(paths[cur_fp], modestr);
			if (!fp) return -1;
			*fp_addrs[cur_fp] = fp;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				key[(char_idx + 13) % 9] = (key[(char_idx + 13) % 9] - 12) % 26;
				key[(char_idx + 20) % 9] = (key[(char_idx + 20) % 9] - 1) % 26;
				key[(char_idx + 16) % 9] = (key[(char_idx + 16) % 9] - 4) % 26;
				char_idx += 2;
			}

			/* Next state */
			if (cur_fp == 0)
			{
				mode += 5;
				cur_fp++;
				state = STATE_OPENFILE;
			}
			else
			{
				char_idx = 0;

				state = STATE_READCHAR;

				/*printf("%d %d %d %d %d %d %d %d %d\n\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8]);

				for (int i = 0; i < 234; i += 26)
				{
					printf("%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n\n", rotors[i+0], rotors[i+1], rotors[i+2], rotors[i+3], rotors[i+4], rotors[i+5], rotors[i+6], rotors[i+7], rotors[i+8], rotors[i+9], rotors[i+10], rotors[i+11], rotors[i+12], rotors[i+13], rotors[i+14], rotors[i+15], rotors[i+16], rotors[i+17], rotors[i+18], rotors[i+19], rotors[i+20], rotors[i+21], rotors[i+22], rotors[i+23], rotors[i+24], rotors[i+25]);
				}*/

				/*for (int i = 0; i < 9; i++)
				{
					for (int j = 0; j < 26; j++)
					{
						printf("%d ", rotors[(i*26) + j]);
					}

					printf("\n");
				}*/
			}
			break;
		/* Read a character from the input file */
		case STATE_READCHAR: ;
			/* Read the character */
			size_t bytes_read = fread(&read_char, 1, 1, *fp_addrs[0]);
			if (bytes_read != 1) return 0;

			/* Uppercase letters, so turn lowercase */
			if (read_char >= 0x41 && read_char <= 0x5a) read_char += 0x20;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				key[(char_idx + 13) % 9] = (key[(char_idx + 13) % 9] - 12) % 26;
				key[(char_idx + 20) % 9] = (key[(char_idx + 20) % 9] - 1) % 26;
				key[(char_idx + 16) % 9] = (key[(char_idx + 16) % 9] - 4) % 26;
				char_idx += 2;
			}

			/* Next state */
			curr = read_char - 0x61;
			round = 0;
			state = STATE_PARSECHAR;
			break;
		/* Parse a character so it is ready to be written out */
		case STATE_PARSECHAR:
			/* ASCII letter, so convert */
			if (read_char >= 0x61 && read_char <= 0x7a)
			{
				/* Done 9 rounds of parsing */
				if (round == buf_sizes[1])
				{
					/* Check for debugger with signals */
					char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
					__asm__("int3");
					if (char_idx % 2 == 1)
					{
						rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
						rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
						rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
						key[(char_idx + 13) % 9] = (key[(char_idx + 13) % 9] - 12) % 26;
						key[(char_idx + 20) % 9] = (key[(char_idx + 20) % 9] - 1) % 26;
						key[(char_idx + 16) % 9] = (key[(char_idx + 16) % 9] - 4) % 26;
						char_idx += 2;
					}

					state = STATE_WRITECHAR;
				}
				/* More rounds to do */
				else
				{
					curr = rotors[(round * 26) + ((curr + key[round]) % 26)] - 0x61;
					round++;
				}
			}
			/* Other character, so leave as-is */
			else
			{
				curr = read_char - 0x61;
				state = STATE_WRITECHAR;
			}

			break;
		/* Write a character to that output file */
		case STATE_WRITECHAR: ;
			/* Write out the character */
			char final_char = curr + 0x61;
			fwrite(&final_char, 1, 1, *fp_addrs[1]);
			fflush(*fp_addrs[1]);
			char_idx += 2;

			/* Next state */
			inc_idx = buf_sizes[1] - 1;
			if (read_char >= 0x61 && read_char <= 0x7a) state = STATE_INCREMENT;
			else state = STATE_READCHAR;

			/* Check for debugger with signals */
			char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
			__asm__("int3");
			if (char_idx % 2 == 1)
			{
				rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
				rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
				rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
				key[(char_idx + 13) % 9] = (key[(char_idx + 13) % 9] - 12) % 26;
				key[(char_idx + 20) % 9] = (key[(char_idx + 20) % 9] - 1) % 26;
				key[(char_idx + 16) % 9] = (key[(char_idx + 16) % 9] - 4) % 26;
				char_idx += 2;
			}

			break;
		/* Increment the key */
		case STATE_INCREMENT: ;
			/* Increment the current position */
			key[inc_idx]++;

			/* Ripple-carry upward if needed */
			if (key[inc_idx] == 26)
			{
				/* Roll over from 26 to 0 */
				key[inc_idx] = 0;

				/* If there's a char below us, retry there */
				if (inc_idx > 0)
				{
					inc_idx--;
				}

				/* Check for debugger with signals */
				char_idx = (char_idx % 2 == 1) ? char_idx : char_idx + 1;
				__asm__("int3");
				if (char_idx % 2 == 1)
				{
					rotors[(char_idx + 4) % 234] = 0x61 + (((rotors[(char_idx + 4) % 234] + 1) - 0x61) % 26);
					rotors[(char_idx + 1) % 234] = 0x61 + (((rotors[(char_idx + 1) % 234] + 5) - 0x61) % 26);
					rotors[(char_idx + 9) % 234] = 0x61 + (((rotors[(char_idx + 9) % 234] + 6) - 0x61) % 26);
					key[(char_idx + 13) % 9] = (key[(char_idx + 13) % 9] - 12) % 26;
					key[(char_idx + 20) % 9] = (key[(char_idx + 20) % 9] - 1) % 26;
					key[(char_idx + 16) % 9] = (key[(char_idx + 16) % 9] - 4) % 26;
					char_idx += 2;
				}
			}
			/* No need, return to READCHAR again */
			else state = STATE_READCHAR;

			/* Next state */
			break;
		default:
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	/* Verify if correct number of args passed */
	if (argc != 3) return -1;

	/* Get the state machine to begin the program logic */
	return encrypt_sm(argv[1], argv[2]);
}
