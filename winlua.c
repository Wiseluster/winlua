#if !defined(__GNUC__) || !defined(__CYGWIN__) && !defined(__MSYS__)
#error Please use gcc for cygwin or msys to build this program.
#endif

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define BINDIR "/usr/bin"
#define ERROR(M, ...) fprintf(stderr, "ERROR: " M, ##__VA_ARGS__)
#define LUA "lua"
#define LUA_PATH BINDIR"/"LUA
#define MINTTY "mintty"
#define MINTTY_PATH BINDIR"/"MINTTY

static void execute(const char *prog, const char *const *args)
{
	pid_t pid;
	int st;
	
	if (access(prog, X_OK))
	{
		ERROR("cannot execute program: %s\n", prog);
		_exit(ENOENT);
	}

	if (!(pid = fork()))
		execvp(prog, (char *const *) args);

	waitpid(pid, &st, 0);

	if ((st = WEXITSTATUS(st)))
	{
		ERROR("program %s exited with value: %d\n", prog, st);
		_exit(st);
	}
}

int main(void)
{
	char buf[LINE_MAX];
	int fd[3];
	ssize_t siz;
	const char *lua_args[] = {LUA, "-v", NULL},
	           *mintty_args[] = {MINTTY, "-t", buf, LUA_PATH, NULL};

	pipe(fd);
	dup2(STDOUT_FILENO, fd[2]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);

	execute(LUA_PATH, lua_args);

	buf[(siz = read(fd[0], buf, LINE_MAX)) > 0 ? siz - 1 : 0] = '\0';

	close(fd[0]);
	dup2(fd[2], STDOUT_FILENO);
	close(fd[2]);

	execute(MINTTY_PATH, mintty_args);

	return 0;
}
