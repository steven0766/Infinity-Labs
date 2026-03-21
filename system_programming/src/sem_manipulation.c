#define _XOPEN_SOURCE 700

#include <stdio.h>      /* printf, fgets, fprintf, perror */
#include <string.h>     /* strcspn, strlen, strcmp, strtok, memset */
#include <stdlib.h>     /* strtol, exit */
#include <sys/sem.h>    /* semget, semctl, semop */
#include <sys/ipc.h>    /* ftok */
#include <assert.h>     /* assert */
#include <fcntl.h>      /* open */
#include <sys/stat.h>   /* mkdir */
#include <errno.h>      /* errno */
#include <unistd.h>     /* close */

#include "utils.h"      /* EXIT_IF_BAD, TRUE */

#define SEM_FOLDER_PATH "/tmp/sem"

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

typedef struct
{
	char op;
	int amount;
	int undo;
} cmd_ty;

enum
{
	MAX_LEN = 256,
	SEM_NUM = 0
};

static void ReadInputIMP(char *buffer_);
static void ParseCmdIMP(char *user_input_, cmd_ty *cmd_);
static void HandleCommandIMP(int sem_id_, cmd_ty *cmd_);

static void ValidateNameIMP(const char *name_);
static key_t GetKeyIMP(const char *name_);
static int CreateSemIMP(key_t key_);
static void InitSemIMP(int sem_id_);
static void OperateIMP(int sem_id_, int amount_, int undo_);
static void PrintSemValIMP(int sem_id_);
static int ParseNumberIMP(const char *str_, int *out_num_);

int main(int argc, char *argv[])
{
	int sem_id = 0;
	char user_input[MAX_LEN] = {0};
	key_t key = 0;
	cmd_ty cmd = {0};

	EXIT_IF_BAD(2 == argc, 1, "Usage: ./sem_manipulation <name>");

	ValidateNameIMP(argv[1]);

	key = GetKeyIMP(argv[1]);
	sem_id = CreateSemIMP(key);

	printf("Semaphore ready. Commands:\n");
	printf("  D <number> [undo]\n");
	printf("  I <number> [undo]\n");
	printf("  V\n");
	printf("  X\n");

	while (TRUE)
	{
		memset(&cmd, 0, sizeof(cmd));

		ReadInputIMP(user_input);
		ParseCmdIMP(user_input, &cmd);
		HandleCommandIMP(sem_id, &cmd);
	}

	return 0;
}

static void ValidateNameIMP(const char *name_)
{
	assert(name_);

	EXIT_IF_BAD('\0' != name_[0], 1, "Invalid name");
	EXIT_IF_BAD(strlen(name_) == strcspn(name_, "/\\%$;<>\t\n "), 1, "Invalid name");
}

static key_t GetKeyIMP(const char *name_)
{
	char path[MAX_LEN] = {0};
	key_t ret = 0;
	int fd = 0;
	int status = 0;

	assert(name_);

	status = mkdir(SEM_FOLDER_PATH, 0777);
	if (-1 == status)
	{
		EXIT_IF_BAD(EEXIST == errno, 1, "mkdir failed");
	}

	snprintf(path, MAX_LEN, "%s/%s", SEM_FOLDER_PATH, name_);

	fd = open(path, O_CREAT | O_RDWR, 0666);
	EXIT_IF_BAD(-1 != fd, 1, "open failed");

	EXIT_IF_BAD(0 == close(fd), 1, "close failed");

	ret = ftok(path, 'S');
	EXIT_IF_BAD((key_t)-1 != ret, 1, "ftok failed");

	return ret;
}

static int CreateSemIMP(key_t key_)
{
	int ret = 0;

	ret = semget(key_, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if (-1 == ret)
	{
		if (EEXIST == errno)
		{
			ret = semget(key_, 1, 0666);
			EXIT_IF_BAD(-1 != ret, 1, "semget open failed");
			return ret;
		}

		EXIT_IF_BAD(0, 1, "semget create failed");
	}

	InitSemIMP(ret);

	return ret;
}

static void InitSemIMP(int sem_id_)
{
	union semun arg;
	int status = 0;

	arg.val = 0;

	status = semctl(sem_id_, SEM_NUM, SETVAL, arg);
	EXIT_IF_BAD(-1 != status, 1, "semctl SETVAL failed");
}

static void ReadInputIMP(char *buffer_)
{
	assert(buffer_);

	printf("> ");
	fflush(stdout);

	EXIT_IF_BAD(NULL != fgets(buffer_, MAX_LEN, stdin), 1, "fgets failed");

	buffer_[strcspn(buffer_, "\n")] = '\0';
}

static int ParseNumberIMP(const char *str_, int *out_num_)
{
	char *endptr = NULL;
	long value = 0;

	assert(str_);
	assert(out_num_);

	errno = 0;
	value = strtol(str_, &endptr, 10);

	if ('\0' == str_[0] || '\0' != *endptr || 0 != errno)
	{
		return 1;
	}

	if (0 > value || 32767 < value)
	{
		return 1;
	}

	*out_num_ = (int)value;
	return 0;
}

static void ParseCmdIMP(char *user_input_, cmd_ty *cmd_)
{
	char *token = NULL;

	assert(user_input_);
	assert(cmd_);

	token = strtok(user_input_, " ");
	EXIT_IF_BAD(NULL != token, 1, "command can't be empty");

	EXIT_IF_BAD('\0' != token[0], 1, "invalid command");
	cmd_->op = token[0];

	switch (cmd_->op)
	{
		case 'V':
		case 'X':
		{
			token = strtok(NULL, " ");
			EXIT_IF_BAD(NULL == token, 1, "too many arguments");
			return;
		}

		case 'I':
		case 'D':
		{
			token = strtok(NULL, " ");
			EXIT_IF_BAD(NULL != token, 1, "number is required");
			EXIT_IF_BAD(0 == ParseNumberIMP(token, &cmd_->amount), 1, "invalid number");

			token = strtok(NULL, " ");
			if (NULL != token)
			{
				EXIT_IF_BAD(0 == strcmp(token, "undo"), 1, "third parameter must be 'undo'");
				cmd_->undo = 1;

				token = strtok(NULL, " ");
				EXIT_IF_BAD(NULL == token, 1, "too many arguments");
			}

			return;
		}

		default:
		{
			EXIT_IF_BAD(0, 1, "invalid command");
		}
	}
}

static void HandleCommandIMP(int sem_id_, cmd_ty *cmd_)
{
	assert(cmd_);

	switch (cmd_->op)
	{
		case 'I':
			OperateIMP(sem_id_, cmd_->amount, cmd_->undo);
			printf("ok, value = ");
			PrintSemValIMP(sem_id_);
			break;

		case 'D':
			OperateIMP(sem_id_, -cmd_->amount, cmd_->undo);
			printf("ok, value = ");
			PrintSemValIMP(sem_id_);
			break;

		case 'V':
			PrintSemValIMP(sem_id_);
			break;

		case 'X':
			printf("Exiting\n");
			exit(0);

		default:
			fprintf(stderr, "invalid command: %c\n", cmd_->op);
			break;
	}
}

static void OperateIMP(int sem_id_, int amount_, int undo_)
{
	int status = 0;
	struct sembuf sop = {0};

	sop.sem_num = SEM_NUM;
	sop.sem_op = (short)amount_;
	sop.sem_flg = undo_ ? SEM_UNDO : 0;

	status = semop(sem_id_, &sop, 1);
	EXIT_IF_BAD(0 == status, 1, "semop failed");
}

static void PrintSemValIMP(int sem_id_)
{
	int val = semctl(sem_id_, SEM_NUM, GETVAL);

	EXIT_IF_BAD(-1 != val, 1, "semctl GETVAL failed");

	printf("%d\n", val);
}
