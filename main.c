#include "monty.h"
/**
 * main - Entry point
 * @argc: Number of arguments
 * @argv: Arguments
 * Return: number of arguments.
 */
int main(int argc, char *argv[])
{
	if (argc >= 2 && argc <= 3)
		handle_command(argv[1]);
	else
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}
}
/**
 * handle_command - Read file
 * @argv: Arguments
 * Return: Nothing
 */
void handle_command(char *argv)
{
	int count = 0, result = 0;
	FILE *fd;
	size_t bufsize = 0;
	char *line = NULL, *arguments = NULL, *item = NULL;
	stack_t *stack = NULL;

	fd = fopen(argv, "r");
	if (fd)
	{
		while (getline(&line, &bufsize, fd) != -1)
		{
			count++;
			arguments = strtok(line, " \n\t\r");
			if (arguments == NULL)
			{
				free(arguments);
				continue;
			}
			item = strtok(NULL, " \n\t\r");
			result = get_opc(&stack, arguments, item, count);
			if (result == 1)
				push_error(fd, line, stack, count);
			else if (result == 2)
				ins_error(fd, line, stack, arguments, count);
		}
		free(line);
		free_dlistint(stack);
		fclose(fd);
	}
	else
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv);
		exit(EXIT_FAILURE);
	}
}
/**
 * get_opc - function to handle the opcode
 * @stack: is a stack or queue
 * @arg: is a parameter
 * @item: is a parameter
 * @count: is a line command
 * Return: nothing
 */
int get_opc(stack_t **stack, char *arg, char *item, int count)
{
	int i = 0, value;

	instruction_t op[] = {
		{"pall", _pall},
		{"pint", _pint},
		{NULL, NULL}
	};

	if (!strcmp(arg, "push"))
	{
		if (_isdigit(item) == 1)
		{
			value = atoi(item);
			_push(stack, value);
			return (0);
		}
		else
			return (1);
	}

	while (op[i].opcode)
	{
		if (!strcmp(arg, op[i].opcode))
		{
			op[i].f(stack, (unsigned int)count);
			break;
		}
		i++;
	}
	if (!op[i].opcode)
		return (2);

	return (0);
}