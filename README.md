*This project has been created as part of the 42 curriculum by nguerrab.*

# get_next_line

## Description

**get_next_line** is a C function that returns **one line at a time** from a file descriptor. Each call returns the next line, until there is nothing left to read.

```c
char	*get_next_line(int fd);
```

The goal of the project is to learn about **static variables**: a static variable keeps its value between function calls, which lets the function remember what it has already read.

The project has two parts:

- **Mandatory**: reads one file descriptor at a time, using one static variable.
- **Bonus**: same function, but it can handle **several file descriptors at the same time**, still using **only one static variable**.

### Behavior

- Returns the line that was read, **including the terminating `\n`**.
- The last line has no `\n` if the file does not end with one.
- Returns `NULL` when there is nothing left to read, or if an error occurs.
- Works with regular files and with standard input.
- Reads as little as possible on each call (it never reads the whole file at once).
- *(Bonus)* Keeps a separate reading state for each file descriptor, so calls on different `fd`s can be mixed freely.

### Files

| File | Part | Content |
|---|---|---|
| `get_next_line.c` | Mandatory | The main function `get_next_line()`. |
| `get_next_line_utils.c` | Mandatory | Helper functions used by the main function. |
| `get_next_line.h` | Mandatory | Prototypes and the default `BUFFER_SIZE`. |
| `get_next_line_bonus.c` | Bonus | `get_next_line()` with multi-fd support. |
| `get_next_line_utils_bonus.c` | Bonus | Helper functions for the bonus version. |
| `get_next_line_bonus.h` | Bonus | Prototypes, default `BUFFER_SIZE` and the maximum number of file descriptors. |

### Technical constraints

- Written in C, following the 42 Norm, compiled with `-Wall -Wextra -Werror`.
- Allowed external functions: `read`, `malloc`, `free`.
- Forbidden: `libft`, `lseek()` and global variables.
- The project must compile with and without `-D BUFFER_SIZE=n`.
- *(Bonus)* Only **one** static variable is allowed.

## Instructions

### Compilation

**Mandatory part**, with a chosen buffer size:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Without the flag, the default value defined in `get_next_line.h` is used:

```bash
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c main.c -o gnl
```

**Bonus part**, with a chosen buffer size:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
```

Without the flag, the default value defined in `get_next_line_bonus.h` is used:

```bash
cc -Wall -Wextra -Werror get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
```

> `main.c` and `main_bonus.c` are only test programs, they are not part of the submitted files.

### Usage

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

Each returned line is allocated with `malloc`, so the caller must `free` it.

### Usage (bonus: several file descriptors)

With the bonus version, calls on different file descriptors can be interleaved. Each `fd` keeps its own position:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

int	main(void)
{
	int		fd1;
	int		fd2;
	char	*line1;
	char	*line2;

	fd1 = open("file1.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
		return (1);
	line1 = get_next_line(fd1);
	line2 = get_next_line(fd2);
	while (line1 || line2)
	{
        if (line1)
        {
		    printf("%s", line1);
		    free(line1);
        }
        line1 = get_next_line(fd1);
        if (line2)
        {
		    printf("%s", line2);
		    free(line2);
        }
		line2 = get_next_line(fd2);
	}
	free(line1);
	free(line2);
	close(fd1);
	close(fd2);
	return (0);
}
```

### Testing tips

- Try several buffer sizes: `1`, `42`, `9999`, `10000000`.
- Try files with: an empty file, one line without `\n`, very long lines, empty lines, no trailing `\n`.
- Try reading from standard input (`fd = 0`).
- File descriptors do not only point to regular files: also try a pipe or a FIFO.
- *(Bonus)* Read from two or three file descriptors alternately (`3`, `4`, `5`, `3`, `4`, ...) and check that no line is lost or mixed up.
- *(Bonus)* Finish one file descriptor before the others, then keep reading the remaining ones.
- *(Bonus)* Test an invalid `fd` (`-1`) and an `fd` above the supported maximum: the function must return `NULL` without crashing.

## Algorithm


### How it works

The function uses **one static variable** (a "stash") that stores the characters already read from the file but not yet returned.

On each call:

1. **Check the input**: if `fd` is invalid or `BUFFER_SIZE` is not positive, return `NULL`.
2. **Read**: while the stash does not contain a `\n`, call `read()` with a buffer of `BUFFER_SIZE` bytes and append what was read to the stash.
3. **Stop reading** as soon as a `\n` is found in the stash, or when `read()` returns `0` (end of file).
4. **Extract the line**: copy from the start of the stash up to and including the first `\n` (or to the end if there is none).
5. **Update the stash**: keep only the characters after the returned line, for the next call.
6. **Clean up**: if the stash becomes empty, free it and set it to `NULL`. If `read()` fails, free the stash and return `NULL`.

### Why this approach

- **Static stash**: it is the only way to keep the leftover characters between two calls without a global variable (which is forbidden).
- **Read only until a `\n` is found**: the subject asks to read as little as possible, so the function never loads the whole file.
- **Works with any `BUFFER_SIZE`**: with a small buffer (like `1`) the function simply loops more times. With a big buffer, the extra characters read are kept in the stash for the next calls.
- **Memory safety**: the stash is freed as soon as it is empty or on error, so no memory leaks remain after the end of the file.

### Bonus: multiple file descriptors


The bonus keeps the same algorithm as the mandatory part, with one change: the single static variable is no longer one stash, but an **array of stashes indexed by the file descriptor**.

```c
static char	*stash[FD_MAX];   /* one static variable, one stash per fd */
```

On each call, `stash[fd]` is used in place of the single stash. The six steps above stay the same.

Why this works:

- **Still one static variable**: the array is a single variable, so the bonus rule ("only one static variable") is respected.
- **Independent states**: each `fd` has its own leftover characters, so reading `fd 3` never touches the data of `fd 4`. This is what allows calls like `3, 4, 5, 3, 4` without losing the reading position.
- **Direct access**: using the `fd` as the array index gives constant-time access, with no search needed (unlike a linked list of `fd`s).
- **Bounds check**: if `fd < 0` or `fd >= FD_MAX`, the function returns `NULL` instead of reading outside the array.
- **Clean-up per fd**: when the stash of one `fd` is empty or `read()` fails, only `stash[fd]` is freed and set to `NULL`. The other file descriptors are not affected.

Trade-off: the array has a fixed size (`FD_MAX`), so it limits the highest `fd` that can be used. In exchange, the code stays simple and fast.

## Resources

### References

- `man 2 read`, `man 3 malloc`, `man 3 free`
- Static variables in C (storage duration and scope)
- [The 42 Norm](https://github.com/42School/norminette)
- *The C Programming Language* by Brian W. Kernighan and Dennis M. Ritchie
- Peer discussions and peer evaluations with other 42 students

### Use of AI

AI was used to help generate the structure and wording of this `README.md` file, including the bonus section. 
