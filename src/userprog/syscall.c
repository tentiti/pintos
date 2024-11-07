#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

// project02
#include "filesys/filesys.h"
#include <stdbool.h>

static void syscall_handler(struct intr_frame *);

void syscall_init(void)
{
    intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void is_valid_vaddr(const void *pointer)
{
    if (pointer == NULL || !is_user_vaddr(pointer) ||
        pagedir_get_page(thread_current()->pagedir, pointer) == NULL)
    {
        exit(-1);
    }
}

static void is_valid_fd(int fdPos)
{
    if (thread_current()->fd[fdPos] == NULL)
    {
        exit(-1);
    }
}

static void syscall_handler(struct intr_frame *f)
{

    int *esp = f->esp;
    is_valid_vaddr(esp);

    int syscall_number = *esp;
    switch (syscall_number)
    {

        // project 01
    case SYS_HALT:
        halt();
        break;

    case SYS_EXIT:
        is_valid_vaddr(esp + 1);
        exit(*(esp + 1));
        break;

    case SYS_EXEC:
        is_valid_vaddr(esp + 1);
        f->eax = exec(*(const char **)(esp + 1));
        break;

    case SYS_WAIT:
        is_valid_vaddr(esp + 1);
        f->eax = wait(*(pid_t *)(esp + 1));
        break;

    case SYS_WRITE:
        is_valid_vaddr(esp + 5);
        f->eax = write(*(int *)(esp + 1), *(const void **)(esp + 2), *(unsigned *)(esp + 3));
        break;

    case SYS_READ:
        is_valid_vaddr(esp + 5);
        f->eax = read(*(int *)(esp + 1), *(void **)(esp + 2), *(unsigned *)(esp + 3));
        break;

    case SYS_FIBONACCI:
        is_valid_vaddr(esp + 1);
        f->eax = fibonacci(*(int *)(esp + 1));
        break;

    case SYS_MAX_OF_FOUR_INT:
        is_valid_vaddr(esp + 4);
        f->eax = max_of_four_int(*(int *)(esp + 1), *(int *)(esp + 2), *(int *)(esp + 3), *(int *)(esp + 4));
        break;

        // project 02
    case SYS_CREATE:
    {
        const char *file = *(const char **)(f->esp + 4);   // 첫 번째 인자: 파일 이름
        unsigned initial_size = *(unsigned *)(f->esp + 8); // 두 번째 인자: 초기 크기
        is_valid_vaddr(file);                              // 파일 이름의 유효성 확인
        f->eax = create(file, initial_size);               // 반환 값을 eax에 저장
        break;
    }

    case SYS_REMOVE:
    {
        const char *file = *(const char **)(f->esp + 4); // 첫 번째 인자: 파일 이름
        is_valid_vaddr(file);                            // 파일 이름의 유효성 확인
        f->eax = remove(file);                           // 반환 값을 eax에 저장
        break;
    }

    case SYS_OPEN:
    {
        const char *file = *(const char **)(f->esp + 4); // 첫 번째 인자: 파일 이름
        is_valid_vaddr(file);                            // 파일 이름의 유효성 확인
        f->eax = open(file);                             // 반환 값을 eax에 저장
        break;
    }

    case SYS_CLOSE:
    {
        int fd = *(int *)(f->esp + 4); // 첫 번째 인자: 파일 디스크립터
        close(fd);                     // 반환 값이 필요하지 않음 (void 함수)
        break;
    }

    case SYS_FILESIZE:
    {
        int fd = *(int *)(f->esp + 4); // 첫 번째 인자: 파일 디스크립터
        f->eax = filesize(fd);         // 반환 값을 eax에 저장
        break;
    }

    case SYS_SEEK:
    {
        int fd = *(int *)(f->esp + 4);                 // 첫 번째 인자: 파일 디스크립터
        unsigned position = *(unsigned *)(f->esp + 8); // 두 번째 인자: 위치
        seek(fd, position);                            // 반환 값이 필요하지 않음 (void 함수)
        break;
    }

    case SYS_TELL:
    {
        int fd = *(int *)(f->esp + 4); // 첫 번째 인자: 파일 디스크립터
        f->eax = tell(fd);             // 반환 값을 eax에 저장
        break;
    }

    default:
        // printf("Unknown system call: %d\n", syscall_number);
        exit(-1);
        break;
    }
}

void halt()
{
    // printf("halt called.\n");
    shutdown_power_off();
}

void exit(int status)
{
    struct thread *cur = thread_current();
    cur->exit_status = status;
    printf("%s: exit(%d)\n", cur->name, status);
    thread_exit();
}

pid_t exec(const char *cmd_line)
{
    return process_execute(cmd_line);
}

int wait(pid_t pid)
{
    return process_wait(pid);
}

int write(int fd, const void *buffer, unsigned size)
{
    is_valid_vaddr(buffer);
    // printf("writing\n");
    if (fd == 1)
    { // File descriptor 1 is stdout
        putbuf(buffer, size);
        return size;
    }
    else
    {
        // Implement file writing if required
        return -1;
    }
}

int read(int fd, void *buffer, unsigned size)
{
    is_valid_vaddr(buffer);
    if (fd == 0)
    { // File descriptor 0 is stdin
        unsigned i;
        for (i = 0; i < size; i++)
        {
            if (!input_getc(buffer + i))
            {
                break;
            }
        }
        return i;
    }
    else
    {
        // Implement file reading if required
        return -1;
    }
}

int fibonacci(int n)
{
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 1;
    else
    {
        int a = 0, b = 1, temp, i;
        for (i = 2; i <= n; i++)
        {
            temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }
}

int max_of_four_int(int a, int b, int c, int d)
{
    int max = a;
    if (b > max)
        max = b;
    if (c > max)
        max = c;
    if (d > max)
        max = d;
    return max;
}

// project02

bool create(const char *file, unsigned initial_size)
{
    return filesys_create(file, initial_size);
}

bool remove(const char *file)
{
    return filesys_remove(file);
}

int open(const char *file)
{
}
void close(int fd)
{
}
int filesize(int fd)
{
}
void seek(int fd, unsigned position)
{
}
unsigned tell(int fd)
{
}

// read, write -> improve from project 01.