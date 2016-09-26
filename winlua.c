#if !defined(__GNUC__) || !defined(__CYGWIN__) && !defined(__MSYS__)
#error Please use gcc for cygwin or msys2 to build this program.
#endif

#define PREFIX "/usr/bin/"
#define LUA "lua"
#define MINTTY "mintty"
#define LUA_PATH PREFIX LUA
#define MINTTY_PATH PREFIX MINTTY

#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

typedef char *const args_t[];

static void safe_execute(const char *prog, args_t args)
{
	pid_t pid;
	int st;
	
	if (access(prog, X_OK))
	{
		fprintf(stderr, "Cannot find program: %s\n", prog);
		_exit(ENOENT);
	}

	if (!(pid = fork()))
		execvp(prog, args);

	waitpid(pid, &st, 0);

	if (!WIFEXITED(st))
		_exit(WEXITSTATUS(st));
}

int main(void)
{
	char buf[LINE_MAX];
	int fd[2];
	args_t lua_args = {LUA, "-v", NULL},
	       mintty_args = {MINTTY, "-t", buf, LUA_PATH, NULL};

	pipe(fd);
	dup2(fd[1], STDOUT_FILENO);

	safe_execute(LUA_PATH, lua_args);

	read(fd[0], buf, LINE_MAX);
	close(fd[0]);
	close(fd[1]);

	safe_execute(MINTTY_PATH, mintty_args);

	return 0;
}
