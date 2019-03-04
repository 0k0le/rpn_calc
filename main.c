// C Project Challenge #45
// Matthew Todd Geiger
// 2019-03-03 21:52:00

// The RPN Calculator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define ASCII_NUM_START 48
#define ASCII_NUM_END 57

int get_level(int *levels, char sym)
{

    switch (sym)
    {
    case '+':
        return levels[0];
        break;
    case '-':
        return levels[1];
        break;
    case '*':
        return levels[2];
        break;
    case '/':
        return levels[3];
        break;
    default:
        return 0;
        break;
    }
}

int sort_operators(char *buf, int *levels, char sym, int *index, int p, char *pbuf)
{
    int ret = -1;

    if (*index > 64)
        goto out;

    // If operator is in parenthesis
    if (p != 0)
    {
        char t[3];

        memcpy(t, &sym, 1);
        t[1] = 0;

        strcat(pbuf, t);

        *index = *index + 1;

        ret = 0;

        goto out;
    }

    int level = get_level(levels, sym);
    printf("[DEBUG] %c Level Value: %d\n", sym, level);

    int i = 0;

    // If empty, resolve
    if (!buf[i])
    {
        buf[i] = sym;
        buf[i + 1] = 0;

        *index = *index + 1;

        ret = 0;

        goto out;
    }

    // Cycle through buffer until the operator finds it's pemdas position
    while (get_level(levels, buf[i]) > level)
    {
        printf("[LOOP 1] Detected: %c @ %d\n", buf[i], i);
        if (i > 64)
        {
            fprintf(stderr, "[ERR DEBUG]: Buffer overflow protection\n");
            goto out;
        }

        i++;
    }

    char temp[64];

    memcpy(temp, buf + i, *index - i);
    buf[i] = sym;
    memcpy(buf + (i + 1), temp, *index - i);
    buf[*index + 1] = 0;

    *index = *index + 1;

    ret = 0;

out:
    return ret;
}

int rpn_sort(const char *equation, char *buffer)
{
    int ret = -1;
    int length = strlen(equation);
    int parenthesis = 0;
    int index = 0;

    char operator_buffer[64];
    int operator_index = 0;

    char pbuffer[64];

    buffer[0] = 0;
    buffer[1] = 0;

    memset(operator_buffer, 0, 64);

    //               +  -  *  /
    int levels[4] = {1, 1, 3, 4};

    for (int i = 0; i < length; i++)
    {
        if ((int)equation[i] < ASCII_NUM_START || (int)equation[i] > ASCII_NUM_END)
        {
            if (equation[i] == ' ')
            {
                i++;
            }
            else if (equation[i] == '(')
            {
                if (parenthesis > 0)
                {
                    fprintf(stderr, "[ERR DEBUG]: Invalid use of parenthesis: %c \n", equation[i]);
                    goto out;
                }
            }
            else if (equation[i] == ')')
            {
                if (parenthesis < 0)
                {
                    fprintf(stderr, "[ERR DEBUG]: Invalid use of parenthesis: %c \n", equation[i]);
                    goto out;
                }
            }
            else
            {
                fprintf(stderr, "[ERR DEBUG]: Invalid Char Detected: %c \n", equation[i]);
                goto out;
            }
        }

        switch (equation[i])
        {
        case '+':
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);
            sort_operators(operator_buffer, levels, equation[i], &operator_index, parenthesis, pbuffer);
            buffer[index] = ' ';
            buffer[index + 1] = 0;

            index++;
            break;
        case '-':
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);
            sort_operators(operator_buffer, levels, equation[i], &operator_index, parenthesis, pbuffer);
            buffer[index] = ' ';
            buffer[index + 1] = 0;

            index++;
            break;
        case '*':
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);
            sort_operators(operator_buffer, levels, equation[i], &operator_index, parenthesis, pbuffer);
            buffer[index] = ' ';
            buffer[index + 1] = 0;

            index++;;
            break;
        case '/':
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);
            sort_operators(operator_buffer, levels, equation[i], &operator_index, parenthesis, pbuffer);
            buffer[index] = ' ';
            buffer[index + 1] = 0;

            index++;
            break;
        case '(':
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);

            parenthesis++;
            buffer[index] = equation[i];
            buffer[index + 1] = 0;

            index++;
            break;
        case ')':
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);

            parenthesis--;
            buffer[index] = equation[i];
            buffer[index + 1] = 0;

            index++;
            break;
        default:
            printf("Detected %c | pval: %d\n", equation[i], parenthesis);

            buffer[index] = equation[i];
            buffer[index + 1] = 0;

            index++;

            break;
        }

        operator_buffer[strlen(operator_buffer) + 1] = 0;
    }
    if (parenthesis != 0)
    {
        fprintf(stderr, "[ERR DEBUG]: Invalid pval: %d\n", parenthesis);
        goto out;
    }

    memset(buffer + index, ' ', 1);
    strcat(buffer, pbuffer);
    strcat(buffer, operator_buffer);

    ret = 0;

out:
    return ret;
}

int main(const int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage %s <EQUATION>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *equation = argv[1];
    char buffer[256];

    if (rpn_sort(equation, buffer) != 0)
    {
        fprintf(stderr, "Failed to create rpn\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buffer);

    exit(EXIT_SUCCESS);
}